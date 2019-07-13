open BsNode;
open BsLodash;
open BsRimRaf;
open BsMkdirp;
open BsDiskUsage;

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
    Firebase.Database.set(
      foldersDB,
      {"list": Js.Array.joinWith(",", folders)},
    )
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
    downloadedContent := downloadedContent^ + chunk##length;
    updateProgress({"id": data##id, "progress": downloadedContent^});
    ();
  };
  let onResponse = res => {
    let contentLength =
      Download.getContentLength(res##headers, "content-length")
      ->Js.Nullable.toOption
      ->Belt.Option.getWithDefault(0);

    let filename = Download.getFilename(res);

    Firebase.Database.update(
      fileInstance,
      {
        "size": contentLength,
        "status": Download.statusToJs(`pending),
        "filename": filename,
      },
    )
    |> ignore;
    ();
  };
  let onEnd = () => {
    Firebase.Database.update(
      fileInstance,
      {
        "status": Download.statusToJs(`completed),
        "progress": downloadedContent^,
      },
    )
    |> ignore;
    ();
  };
  let onError = err => {
    Js.log(err);
    ();
  };
  Download.download(
    ~url=data##url,
    ~folderPath=downloadFolderPath,
    ~extract=data##extract,
    ~filename=data##filename,
    ~onData,
    ~onResponse,
    ~onEnd,
    ~onError,
  )
  |> Js.Promise.then_(data => {
       Firebase.Database.update(fileInstance, {"filepath": data##filepath})
       |> ignore;
       Js.Promise.resolve();
     })
  |> ignore;
  ();
};

Firebase.Database.on(
  filesDB,
  ~eventType="child_added",
  ~callback=snapshot => {
    let data = Firebase.DataSnapshot.val_(snapshot);
    let status = Download.statusFromJs(data##status);
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
  },
);

Firebase.Database.on(
  filesDB,
  ~eventType="child_removed",
  ~callback=snapshot => {
    let data = Firebase.DataSnapshot.val_(snapshot);
    removeFolder(data##filepath, () => ());
  },
);