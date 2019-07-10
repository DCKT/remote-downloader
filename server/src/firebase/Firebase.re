module Error = {
  type t('e) = Js.t('e);
};

module DataSnapshot = {
  type t;
  [@bs.send] external val_: t => Js.t('a) = "val";
};

module Database = {
  type t;
  [@bs.send] external ref: (t, string) => t = "";
  [@bs.send] external set: (t, 'a) => t = "";
  [@bs.send] external update: (t, Js.t('a)) => t = "";
  [@bs.send] external child: (t, string) => t = "";
  type cb = DataSnapshot.t => unit;
  [@bs.send] external on: (t, ~eventType: string, ~callback: cb) => cb = "";
};

module App = {
  type t;
  [@bs.send] external database: t => Database.t = "";
};

[@bs.module "firebase-admin"] external initializeApp: 'a => App.t = "";

type credentialInterface = {. [@bs.meth] "cert": string => string};

[@bs.module "firebase-admin"] external credential: credentialInterface = "";

[@bs.module] external devConfig: string = "./data/dev.json";

[@bs.module] external prodConfig: string = "./data/prod.json";

let nodeEnv: option(string) = Some([%raw "process.env.NODE_ENV"]);

let config =
  switch (nodeEnv) {
  | Some("production") => {
      "credential": credential##cert(prodConfig),
      "databaseURL": "https://terebi-a2a7b.firebaseio.com",
    }
  | _ => {
      "credential": credential##cert(devConfig),
      "databaseURL": "https://terebi-dev.firebaseio.com",
    }
  };

let app = initializeApp(config);