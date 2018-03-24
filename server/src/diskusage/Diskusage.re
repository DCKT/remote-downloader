type output = {
  .
  "available": int,
  "free": int,
  "total": int,
};

[@bs.module "diskusage"] external checkDisk : ('a, 'b) => unit = "check";

let getDiskSpace: (~path: string) => Js.Promise.t(output) =
  (~path: string) =>
    Js.Promise.make((~resolve, ~reject) =>
      checkDisk(path, (err, info) =>
        switch (err) {
        | None => resolve(. info)
        | Some(error) => reject(. error)
        }
      )
    );