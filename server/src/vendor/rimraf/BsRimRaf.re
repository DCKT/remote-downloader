type callback = unit => unit;

[@bs.module] external removeFolder: (string, callback) => unit = "rimraf";