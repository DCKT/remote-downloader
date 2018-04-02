[@bs.deriving jsConverter]
type status = [
  | [@bs.as "waiting"] `waiting
  | [@bs.as "pending"] `pending
  | [@bs.as "completed"] `completed
  | [@bs.as "error"] `error
];

type t = {
  .
  "extract": bool,
  "folder": string,
  "id": string,
  "progress": int,
  "status": string,
  "filename": string,
  "size": int,
};

[@bs.val] external dateNow : unit => int = "Date.now";

let add = (~url: string, ~extract: bool) => {
  let id = dateNow();
  let db = Firebase.App.database(Firebase.app);
  let newRef = Firebase.Database.ref(db, "files/" ++ string_of_int(id));
  Firebase.Database.set(
    newRef,
    Js.Nullable.return({
      "id": id,
      "progress": 0,
      "size": 0,
      "url": url,
      "extract": extract |> Js.Boolean.to_js_boolean,
      "status": "waiting",
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