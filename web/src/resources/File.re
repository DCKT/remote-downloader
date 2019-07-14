[@bs.deriving jsConverter]
type status = [
  | [@bs.as "waiting"] `waiting
  | [@bs.as "pending"] `pending
  | [@bs.as "completed"] `completed
  | [@bs.as "error"] `error
];

[@bs.deriving abstract]
type t = {
  extract: bool,
  folder: string,
  id: string,
  progress: int,
  status: string,
  filename: string,
  size: int,
};

[@bs.val] external dateNow: unit => int = "Date.now";

let add = (~url: string, ~filename: string, ~extract: bool) => {
  let id = dateNow()->string_of_int;

  Firebase.app
  ->Firebase.App.database
  ->Firebase.Database.ref("files/" ++ id)
  ->Firebase.Database.set(
      Js.Nullable.return({
        "id": id,
        "progress": 0,
        "size": 0,
        "url": url,
        "extract": extract,
        "status": "waiting",
        "filename": filename,
      }),
    )
  |> ignore;
};

let remove = (~id: string) => {
  let db = Firebase.App.database(Firebase.app);
  let fileRef = Firebase.Database.ref(db, "files/" ++ id);
  Firebase.Database.set(fileRef, Js.Nullable.null) |> ignore;
  ();
};