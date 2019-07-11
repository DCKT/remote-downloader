[@bs.scope "localStorage"] [@bs.val] [@bs.return nullable]
external getItem : string => option(string) = "getItem";

[@bs.scope "localStorage"] [@bs.val]
external setItem : (string, string) => unit = "setItem";