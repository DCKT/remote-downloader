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

module App = {
  type t;
  [@bs.send] external database : t => Database.t = "";
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