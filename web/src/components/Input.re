let component = "Input" |> ReasonReact.statelessComponent;

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
      _children,
    ) => {
  ...component,
  render: _self => {
    let inputValue =
      switch (value) {
      | None => ""
      | Some(v) => v
      };
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
    let errorField =
      switch (error) {
      | None => ReasonReact.nullElement
      | Some(err) => <div className=Styles.error> err </div>
      };
    let disabledAttr =
      switch (disabled) {
      | None => Js.false_
      | Some(value) => value
      };
    let onBlurEvent =
      switch (onBlur) {
      | None => (_ => ())
      | Some(e) => e
      };

    <div className=customContainerClassName>
      {
        switch (label) {
        | None => ReasonReact.nullElement
        | Some(l) => <label className=Styles.label> l </label>
        }
      }
      <input
        _type={inputTypeToJs(_type)}
        placeholder=customPlaceholder
        className=customInputClassName
        value=inputValue
        disabled=disabledAttr
        onChange
        onBlur=onBlurEvent
      />
      errorField
    </div>;
  },
};