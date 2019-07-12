module Styles = {
  open Css;
  let root =
    style([
      display(`flex),
      justifyContent(center),
      alignItems(center),
      height(px(60)),
      background(hex("fff")),
      borderBottom(px(1), solid, hex("eee")),
    ]);
  let title =
    style([
      fontSize(px(22)),
      textTransform(uppercase),
      fontWeight(`bold),
      color(Colors.grey),
    ]);
  let addButton =
    style([
      position(absolute),
      right(px(20)),
      fontSize(px(20)),
      fontWeight(`bold),
      border(px(2), solid, Colors.grey),
      borderRadius(px(3)),
      color(Colors.grey),
      cursor(`pointer),
    ]);
};

[@react.component]
let make = (~showModal) =>
  <nav className=Styles.root>
    <h1 className=Styles.title> {React.string("Downloads")} </h1>
    <button className=Styles.addButton onClick=showModal>
      {React.string("+")}
    </button>
  </nav>;