type onDownload;

[@bs.deriving jsConverter]
type status = [
  | [@bs.as "waiting"] `waiting
  | [@bs.as "pending"] `pending
  | [@bs.as "completed"] `completed
  | [@bs.as "error"] `error
];

type progressT = {
  .
  "percent": float,
  "transferred": int,
  "total": int,
};

type dl;

type chunk = {. "length": int};

type voidPromise = Js.Promise.t(unit);

type moduleOptions = {. "extract": bool};

type headers;

type res = {. "headers": headers};

[@bs.get_index]
external getContentLength: (headers, string) => Js.Nullable.t(int) = "";

type downloadPromiseResponse = {
  .
  "data": string,
  "filename": string,
  "filepath": string,
};

type t('a) = Js.Promise.t(downloadPromiseResponse);

[@bs.module "download"]
external customDownload:
  (~url: string, ~folderPath: string, ~options: moduleOptions) => t('a) =
  "download";

[@bs.module "download"] external getFilename: 'a => string = "getFilename";

[@bs.send.pipe: t('a)]
external on:
  (
  [@bs.string]
  [
    | `response(res => unit)
    | `data(chunk => unit)
    | `error(string => unit)
    | [@bs.as "end"] `end_(unit => unit)
  ]
  ) =>
  t('a) =
  "";

let download =
    (
      ~url: string,
      ~folderPath: string,
      ~onResponse: res => unit,
      ~onData: chunk => unit,
      ~extract: bool,
      ~onEnd: unit => unit,
      ~onError: string => unit,
    ) =>
  customDownload(~url, ~folderPath, ~options={"extract": extract})
  |> on(`response(onResponse))
  |> on(`data(onData))
  |> on(`end_(onEnd))
  |> on(`error(onError));