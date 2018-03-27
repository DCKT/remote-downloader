let component = ReasonReact.statelessComponent("Loading");

module Styles = {
  open Css;
  let root =
    style([
      background(hex("0288D1")),
      height(pct(100.)),
      display(`flex),
      justifyContent(center),
      alignItems(center),
    ]);
};

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.root>
      <Loader isActive=true size=Large color="#fff" />
    </div>,
};