type callback = option(exn) => unit;

[@bs.module] external mkdirp : (string, callback) => unit = "mkdirp";

let createDownloadFolder = (~path: string) =>
  mkdirp(path, err =>
    switch (err) {
    | None => ()
    | Some(error) => Js.log(error)
    }
  );

let statFiles = (files: array(string), path) =>
  Js.Promise.all(
    Array.map(
      file => {
        let filePath = Node.Path.join([|path, file|]);
        Node.Fs.lstatAsync(~path=filePath)
        |> Js.Promise.then_(statFile =>
             Js.Promise.resolve((file, statFile##isDirectory()))
           );
      },
      files,
    ),
  );

let filterDirectories = (files: array((string, bool))) =>
  files
  |> Js.Array.filter(((_, isDirectory)) => isDirectory)
  |> Array.map(((file, _)) => file)
  |> Js.Promise.resolve;

let getFolders = (~path: string) =>
  Node.Fs.readdirAsync(~path)
  |> Js.Promise.then_(files => statFiles(files, path))
  |> Js.Promise.then_(filterDirectories);