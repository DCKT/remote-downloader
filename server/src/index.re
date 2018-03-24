let unsafeFolderPath: Js.Nullable.t(string) = [%raw
  "process.env.DOWNLOAD_FOLDER_PATH"
];

let downloadFolderPath =
  switch (Js.Nullable.toOption(unsafeFolderPath)) {
  | None => "./download"
  | Some(path) => path
  };

Filesystem.createDownloadFolder(~path=downloadFolderPath);

let db = Firebase.app |> Firebase.App.database;

let filesDB = Firebase.Database.ref(db, "files");

let foldersDB = Firebase.Database.ref(db, "folders");

let diskDB = Firebase.Database.ref(db, "disk");

Filesystem.getFolders(~path=downloadFolderPath)
|> Js.Promise.then_(folders => {
     Firebase.Database.set(
       foldersDB,
       {"list": Js.Array.joinWith(",", folders)},
     )
     |> ignore;
     Js.Promise.resolve();
   })
|> Js.Promise.catch(err => {
     Js.log("Filesystem: something went wrong");
     Js.log(err);
     Js.Promise.resolve();
   });

Diskusage.getDiskSpace(~path="/")
|> Js.Promise.then_(infos => {
     Firebase.Database.set(diskDB, infos) |> ignore;
     Js.Promise.resolve();
   })
|> Js.Promise.catch(err => {
     Js.log("Diskusage : something went wrong");
     Js.log(err);
     Js.Promise.resolve();
   });

let updateProgress =
  Lodash.throttle(
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
    let unsafeContentLength =
      Download.getContentLength(res##headers, "content-length");
    let contentLength =
      switch (Js.Nullable.toOption(unsafeContentLength)) {
      | None => 0
      | Some(value) => value
      };
    Firebase.Database.update(
      fileInstance,
      {"size": contentLength, "status": Download.statusToJs(`pending)},
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
    ~folderPath=data##folder,
    ~extract=data##extract,
    ~onData,
    ~onResponse,
    ~onEnd,
    ~onError,
  )
  |> Js.Promise.then_(data => {
       Firebase.Database.update(
         fileInstance,
         {"filename": data##filename, "filepath": data##filePath},
       )
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
    Rimraf.removeFolder(data##filepath, () => ());
  },
);