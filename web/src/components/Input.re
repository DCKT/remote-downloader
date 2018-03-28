let component = "Input" |> ReasonReact.statelessComponent;

[@bs.deriving jsConverter]
type inputType = [
  | [@bs.as "text"] `Text
  | [@bs.as "password"] `Password
  | [@bs.as "email"] `Email
  | [@bs.as "url"] `Url
];

module Styles = {
  open Css;
  let input =
    style([
      padding2(~v=px(0), ~h=px(10)),
      height(px(36)),
      borderRadius(px(2)),
      border(px(1), solid, hex("ccc")),
      fontSize(px(16)),
    ]);
  let error =
    style([fontSize(px(14)), color(hex("F44336")), marginTop(px(5))]);
};

let make =
    (
      ~value: string,
      ~disabled: Js.boolean,
      ~_type: inputType,
      ~placeholder: option(string)=?,
      ~containerClassName: option(string)=?,
      ~inputClassName: option(string)=?,
      ~onBlur,
      ~onChange,
      ~error,
      _children,
    ) => {
  ...component,
  render: _self => {
    let customContainerClassName =
      switch (containerClassName) {
      | None => ""
      | Some(rules) => rules
      };
    let customInputClassName =
      switch (inputClassName) {
      | None => Styles.input
      | Some(rules) => Styles.input ++ " " ++ rules
      };
    let customPlaceholder =
      switch (placeholder) {
      | None => ""
      | Some(p) => p
      };
    <div className=customContainerClassName>
      <input
        _type=(inputTypeToJs(_type))
        placeholder=customPlaceholder
        className=customInputClassName
        value
        disabled
        onChange
        onBlur
      />
      <div className=Styles.error> error </div>
    </div>;
  },
};