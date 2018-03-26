let component = "Loader" |> ReasonReact.statelessComponent;

type size =
  | Tiny
  | Small
  | Medium
  | Large;

module Styles = {
  open Css;
  let spin =
    Css.keyframes([
      (0, [transform(rotate(deg(45)))]),
      (100, [transform(rotate(deg(405)))]),
    ]);
  let loader = (~size: option(size), ~color: option(string)) => {
    let (containerSize, borderWidth) =
      switch (size) {
      | None => (40, 3)
      | Some(s) =>
        switch (s) {
        | Tiny => (20, 2)
        | Small => (40, 3)
        | Medium => (80, 4)
        | Large => (120, 4)
        }
      };
    let loaderColor =
      switch (color) {
      | None => "#242424"
      | Some(c) => c
      };
    style([
      width(px(containerSize)),
      height(px(containerSize)),
      borderTop(px(borderWidth), solid, rgba(24, 24, 24, 0.4)),
      borderRight(px(borderWidth), solid, rgba(24, 24, 24, 0.4)),
      borderBottom(px(borderWidth), solid, rgba(24, 24, 24, 0.4)),
      borderLeft(px(borderWidth), solid, rgba(24, 24, 24, 1.)),
      unsafe("border-left-color", loaderColor ++ " !important"),
      borderRadius(pct(100.)),
      transform(rotate(deg(45))),
      animationName(spin),
      animationDuration(1000),
      animationIterationCount(infinite),
    ]);
  };
};

let make =
    (
      ~isActive: bool,
      ~size: option(size)=?,
      ~color: option(string)=?,
      ~className: option(string)=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    let defaultClassName = Styles.loader(~size, ~color);
    let style =
      switch (className) {
      | None => defaultClassName
      | Some(s) => defaultClassName ++ " " ++ s
      };
    isActive ? <div className=style /> : ReasonReact.nullElement;
  },
};