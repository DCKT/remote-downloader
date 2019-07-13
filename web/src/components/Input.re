open Belt;

[@bs.deriving jsConverter]
type inputType = [
  | [@bs.as "text"] `Text
  | [@bs.as "password"] `Password
  | [@bs.as "email"] `Email
  | [@bs.as "url"] `Url
  | [@bs.as "checkbox"] `Checkbox
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
  let label =
    style([
      display(block),
      fontSize(px(16)),
      color(hex("69798F")),
      marginBottom(px(5)),
    ]);
  let error =
    style([fontSize(px(14)), color(hex("F44336")), marginTop(px(5))]);
};

[@react.component]
let make =
    (
      ~label=?,
      ~value: option(string)=?,
      ~disabled: option(bool)=?,
      ~_type: inputType,
      ~placeholder: option(string)=?,
      ~containerClassName: option(string)=?,
      ~inputClassName: option(string)=?,
      ~onBlur=?,
      ~onChange,
      ~error=?,
      ~id: option(string)=?,
    ) => {
  let customInputClassName =
    switch (inputClassName) {
    | None => Styles.input
    | Some(rules) => Styles.input ++ " " ++ rules
    };
  let errorField =
    switch (error) {
    | None => React.null
    | Some(err) => <div className=Styles.error> err </div>
    };

  <div className=containerClassName->Belt.Option.getWithDefault("")>
    {
      switch (label) {
      | None => React.null
      | Some(l) => <label className=Styles.label> l </label>
      }
    }
    <input
      onChange
      id=id->Option.getWithDefault("")
      type_={inputTypeToJs(_type)}
      placeholder=placeholder->Option.getWithDefault("")
      className=customInputClassName
      value=value->Option.getWithDefault("")
      disabled=disabled->Option.getWithDefault(false)
      onBlur=onBlur->Option.getWithDefault(_ => ())
    />
    errorField
  </div>;
};