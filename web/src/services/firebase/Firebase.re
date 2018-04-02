module Error = {
  type t('e) = Js.t('e);
};

module DataSnapshot = {
  type t;
  [@bs.send] external val_ : t => Js.t('a) = "val";
};

module Database = {
  type t;
  [@bs.send] external ref : (t, string) => t = "";
  [@bs.send] external set : (t, Js.t('a)) => t = "";
  [@bs.send] external update : (t, Js.t('a)) => t = "";
  [@bs.send] external child : (t, string) => t = "";
  type cb = DataSnapshot.t => unit;
  [@bs.send] external on : (t, ~eventType: string, ~callback: cb) => cb = "";
};

module User = {
  type t = {
    .
    "email": Js.Nullable.t(string),
    "displayName": Js.Nullable.t(string),
  };
};

module Authentication = {
  type t;
  type callback = Js.Nullable.t(User.t) => unit;
  [@bs.send] external onAuthStateChanged : (t, callback) => t = "";
  [@bs.send] external signOut : t => t = "";
  [@bs.send]
  external signInWithEmailAndPassword :
    (t, string, string) => Js.Promise.t(unit) =
    "";
};

module App = {
  type t;
  [@bs.send] external database : t => Database.t = "";
  [@bs.send] external auth : t => Authentication.t = "";
};

module Model = {
  [@bs.deriving jsConverter]
  type status = [
    | [@bs.as "waiting"] `waiting
    | [@bs.as "pending"] `pending
    | [@bs.as "completed"] `completed
    | [@bs.as "error"] `error
  ];
  type file = {
    .
    "extract": bool,
    "folder": string,
    "id": string,
    "progress": int,
    "status": string,
    "filename": string,
    "size": int,
  };
};

[@bs.module "firebase"] external initializeApp : 'a => App.t = "";

[@bs.module] external devConfig : string = "./data/dev.json";

[@bs.module] external prodConfig : string = "./data/prod.json";

let nodeEnv: option(string) = Some([%raw "process.env.NODE_ENV"]);

let config =
  switch (nodeEnv) {
  | Some("production") => prodConfig
  | _ => devConfig
  };

let app = initializeApp(config);

let authInstance = App.auth(app);

let login = (~email, ~password) =>
  Authentication.signInWithEmailAndPassword(authInstance, email, password);

let logout = () => Authentication.signOut(authInstance);

[@bs.val] external dateNow : unit => int = "Date.now";

let addFile = (~url: string, ~extract: bool) => {
  let id = dateNow();
  let db = App.database(app);
  let newRef = Database.ref(db, "files/" ++ string_of_int(id));
  Database.set(
    newRef,
    {
      "id": id,
      "progress": 0,
      "size": 0,
      "url": url,
      "extract": extract |> Js.Boolean.to_js_boolean,
      "status": "waiting",
    },
  );
};