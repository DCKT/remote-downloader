let component = "Navbar" |> ReasonReact.statelessComponent;

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
      fontWeight(700),
      color(Colors.grey),
    ]);
  let addButton =
    style([
      position(absolute),
      right(px(20)),
      fontSize(px(20)),
      fontWeight(700),
      border(px(2), solid, Colors.grey),
      borderRadius(px(3)),
      color(Colors.grey),
      cursor(`pointer),
    ]);
};

let make = (~showModal, _children) => {
  ...component,
  render: _self =>
    <nav className=Styles.root>
      <h1 className=Styles.title>
        (ReasonReact.stringToElement("Downloads"))
      </h1>
      <button className=Styles.addButton onClick=showModal>
        (ReasonReact.stringToElement("+"))
      </button>
    </nav>,
};