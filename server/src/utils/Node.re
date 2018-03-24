type callback('b) = (option(exn), 'b) => unit;

module Fs = {
  type lstatInterface = {. [@bs.meth] "isDirectory": unit => bool};
  [@bs.module "fs"]
  external readdir : (string, callback(array(string))) => unit = "";
  [@bs.module "fs"] external unlink : (string, callback(string)) => unit = "";
  [@bs.module "fs"]
  external lstat : (string, callback(lstatInterface)) => unit = "";
  let readdirAsync = (~path: string) =>
    Js.Promise.make((~resolve, ~reject) =>
      readdir(path, (err, files) =>
        switch err {
        | None => [@bs] resolve(files)
        | Some(error) => [@bs] reject(error)
        }
      )
    );
  let removeFileAsync = (~path: string) =>
    Js.Promise.make((~resolve, ~reject) =>
      unlink(path, (err, _null) =>
        switch err {
        | None => [@bs] resolve("ok")
        | Some(error) => [@bs] reject(error)
        }
      )
    );
  let lstatAsync = (~path: string) =>
    Js.Promise.make((~resolve, ~reject) =>
      lstat(path, (err, file) =>
        switch err {
        | None => [@bs] resolve(file)
        | Some(error) => [@bs] reject(error)
        }
      )
    );
};

module Path = {
  [@bs.module "path"] [@bs.splice]
  external resolve : array(string) => string = "";
  [@bs.module "path"] [@bs.splice]
  external join : array(string) => string = "";
};