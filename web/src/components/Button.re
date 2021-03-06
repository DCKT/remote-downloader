[@bs.deriving jsConverter]
type buttonType = [ | [@bs.as "button"] `Button | [@bs.as "submit"] `Submit];

module Styles = {
  open Css;
  let root = (~backgroundColor: option(string), ~color: option(string)) => {
    let bgColor =
      switch (backgroundColor) {
      | None => "#0288D1"
      | Some(color) => color
      };
    let textColor =
      switch (color) {
      | None => "#fff"
      | Some(color) => color
      };
    style([
      display(`flex),
      unsafe("backgroundColor", bgColor),
      fontSize(px(20)),
      unsafe("color", textColor),
      padding2(~v=px(10), ~h=px(30)),
      borderRadius(px(2)),
      border(px(1), none, hex("0288D1")),
      cursor(`pointer),
      boxShadow(~x=px(0), ~y=px(2), ~blur=px(4), rgba(24, 24, 24, 0.1)),
      alignItems(center),
      justifyContent(center),
      selector(":hover", [opacity(0.9)]),
      selector(":disabled", [opacity(0.6)]),
    ]);
  };
};

[@react.component]
let make =
    (
      ~_type: buttonType,
      ~className: option(string)=?,
      ~backgroundColor: option(string)=?,
      ~color: option(string)=?,
      ~disabled: option(bool)=?,
      ~content: React.element,
      ~onClick=?,
    ) => {
  let defaultClassName = Styles.root(~backgroundColor, ~color);
  let buttonStyle =
    switch (className) {
    | None => defaultClassName
    | Some(style) => defaultClassName ++ " " ++ style
    };
  let onClickEvent =
    switch (onClick) {
    | None => (_event => ())
    | Some(e) => e
    };
  let disabledOption =
    switch (disabled) {
    | None => false
    | Some(v) => v
    };
  <button
    onClick=onClickEvent
    type_={buttonTypeToJs(_type)}
    className=buttonStyle
    disabled=disabledOption>
    content
  </button>;
};