module Styles = {
  open Css;
  let root =
    style([
      background(Colors.pageBackground),
      height(pct(100.)),
      display(`flex),
      justifyContent(center),
      alignItems(center),
    ]);
};

[@react.component]
let make = () =>
  <div className=Styles.root>
    <Loader isActive=true size=Large color="#fff" />
  </div>;