type callback('a) = Js.t('a) => unit;

type throttled('a) = Js.t('a) => unit;

type t;

[@bs.module]
external throttle : (callback('a), int) => throttled('a) = "lodash.throttle";