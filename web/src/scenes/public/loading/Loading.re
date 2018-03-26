let component = ReasonReact.statelessComponent("Loading");

module Styles = {
  open Css;
  let root = style([background(hex("0288D1"))]);
};

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.root>
      (ReasonReact.stringToElement("LOADING"))
    </div>,
};