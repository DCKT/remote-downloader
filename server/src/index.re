open BsNode;
open BsLodash;
open BsRimRaf;
open BsMkdirp;
open BsDiskUsage;
open BsDownload;

let unsafeFolderPath: Js.Nullable.t(string) = [%raw
  "process.env.DOWNLOAD_FOLDER_PATH"
];

let downloadFolderPath =
  unsafeFolderPath
  ->Js.Nullable.toOption
  ->Belt.Option.getWithDefault("./download");

mkdirp(downloadFolderPath, err =>
  switch (err->Js.Nullable.toOption) {
  | None => ()
  | Some(error) =>
    Js.log(error);
    Js.Exn.raiseError("Can't create folder");
  }
);

let db = Firebase.app |> Firebase.App.database;

let filesDB = Firebase.Database.ref(db, "files");

let foldersDB = Firebase.Database.ref(db, "folders");

let diskDB = Firebase.Database.ref(db, "disk");

Fs.readdir(downloadFolderPath, (err, folders) =>
  switch (err->Js.Nullable.toOption) {
  | Some(e) =>
    Js.log("Filesystem: something went wrong");
    Js.log(e);

  | None =>
    foldersDB
    ->Firebase.Database.set({"list": Js.Array.joinWith(",", folders)})
    |> ignore
  }
);

checkDiskUsage("/", (err, infos) =>
  switch (err->Js.Nullable.toOption) {
  | None => Firebase.Database.set(diskDB, infos) |> ignore
  | Some(e) =>
    Js.log("Diskusage : something went wrong");
    Js.log(e);
  }
);

let updateProgress =
  throttle(
    item => {
      let file = Firebase.Database.child(filesDB, item##id);
      Firebase.Database.update(file, {"progress": item##progress}) |> ignore;
    },
    10000,
  );

let handleDownload = data => {
  let fileInstance = Firebase.Database.child(filesDB, data##id);
  let downloadedContent = ref(0);
  let onData = chunk => {
    downloadedContent :=
      downloadedContent^ + chunk->DownloadResponse.Chunk.length;
    updateProgress({"id": data##id, "progress": downloadedContent^});
    ();
  };
  let onResponse = res => {
    let contentLength =
      res
      ->DownloadResponse.headers
      ->DownloadResponse.getContentLength("content-length")
      ->Js.Nullable.toOption
      ->Belt.Option.getWithDefault(0);

    res |> DownloadResponse.on(`data(onData)) |> ignore;

    fileInstance
    ->Firebase.Database.update({
        "size": contentLength,
        "status": DownloadStatus.tToJs(`pending),
        "filename": data##filename,
      })
    |> ignore;
    ();
  };
  let onEnd = () => {
    fileInstance
    ->Firebase.Database.update({
        "status": DownloadStatus.tToJs(`completed),
        "progress": downloadedContent^,
      })
    |> ignore;
    ();
  };
  let onError = err => {
    fileInstance
    ->Firebase.Database.update({"status": DownloadStatus.tToJs(`error)})
    |> ignore;
    Js.log(err);
    ();
  };

  download(
    data##url,
    downloadFolderPath,
    DownloadOptions.t(~extract=data##extract, ~filename=data##filename, ()),
  )
  |> on(`response(onResponse))
  |> on(`end_(onEnd))
  |> on(`error(onError))
  |> Js.Promise.then_(data => {
       fileInstance
       ->Firebase.Database.update({
           "filepath": data->DownloadResponse.filepathGet,
         })
       |> ignore;
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(err => {
       Js.log(err);
       fileInstance
       ->Firebase.Database.update({"status": DownloadStatus.tToJs(`error)})
       |> ignore;
       Js.Promise.resolve();
     })
  |> ignore;
  ();
};

filesDB
->Firebase.Database.on(~eventType="child_added", ~callback=snapshot => {
    let data = Firebase.DataSnapshot.val_(snapshot);
    let status = DownloadStatus.tFromJs(data##status);
    switch (status) {
    | None => ()
    | Some(t) =>
      switch (t) {
      | `waiting => handleDownload(data)
      | `pending
      | `completed
      | `error => ()
      }
    };
    ();
  });

filesDB
->Firebase.Database.on(~eventType="child_removed", ~callback=snapshot => {
    let data = Firebase.DataSnapshot.val_(snapshot);
    removeFolder(data##filepath, () => ());
  });