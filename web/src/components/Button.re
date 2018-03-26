let component = "Button" |> ReasonReact.statelessComponent;

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
      margin4(~top=px(25), ~left=auto, ~right=auto, ~bottom=px(0)),
      unsafe("background-color", bgColor),
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

let make =
    (
      ~_type: buttonType,
      ~className: option(string)=?,
      ~backgroundColor: option(string)=?,
      ~color: option(string)=?,
      ~disabled: Js.boolean,
      ~content: ReasonReact.reactElement,
      _children,
    ) => {
  ...component,
  render: _self => {
    let defaultClassName = Styles.root(~backgroundColor, ~color);
    let buttonStyle =
      switch (className) {
      | None => defaultClassName
      | Some(style) => defaultClassName ++ " " ++ style
      };
    <button _type=(buttonTypeToJs(_type)) className=buttonStyle disabled>
      content
    </button>;
  },
};