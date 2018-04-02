let component = "DownloadItem" |> ReasonReact.statelessComponent;

module Styles = {
  open Css;
  let root =
    style([
      background(Colors.white),
      borderRadius(px(2)),
      boxShadow(~x=px(0), ~y=px(0), ~blur=px(12), rgba(24, 24, 24, 0.12)),
      marginBottom(px(30)),
      padding(px(30)),
      position(relative),
    ]);
  let title =
    style([
      fontSize(px(16)),
      color(Colors.black),
      textTransform(uppercase),
      fontWeight(700),
      marginBottom(px(15)),
    ]);
  let progressBarContainer = style([display(`flex), alignItems(center)]);
  let progressBar =
      (~progress: float, ~status: option(Firebase.Model.status)) => {
    let barColor =
      switch (status) {
      | None => Colors.grey
      | Some(s) =>
        switch (s) {
        | `waiting => Colors.grey
        | `pending => Colors.blue
        | `completed => Colors.green
        | `error => Colors.red
        }
      };
    style([
      width(pct(100.)),
      background(Colors.progressBarBackground),
      height(px(5)),
      borderRadius(px(2)),
      selector(
        ":after",
        [
          unsafe("content", " "),
          display(block),
          width(pct(100.)),
          height(pct(100.)),
          background(barColor),
          borderRadius(px(2)),
          width(pct(progress)),
          transition(~duration=1000, ~timingFunction=`ease, "all"),
        ],
      ),
    ]);
  };
  let progress =
    style([marginLeft(px(15)), fontSize(px(14)), color(Colors.grey)]);
  let deleteButton =
    style([
      position(absolute),
      top(px(0)),
      left(pct(100.)),
      cursor(`pointer),
      background(none),
      border(px(0), none, Colors.black),
      opacity(0.5),
      selector(":hover", [opacity(1.)]),
    ]);
  let deleteIcon = style([color(Colors.grey)]);
};

let make = (~file: Firebase.Model.file, _children) => {
  ...component,
  render: _self => {
    let progress =
      float_of_int(file##progress) /. float_of_int(file##size) *. 100.;
    <div className=Styles.root>
      <button className=Styles.deleteButton>
        <i className=("material-icons " ++ Styles.deleteIcon)>
          (ReasonReact.stringToElement("delete_forever"))
        </i>
      </button>
      <h1 className=Styles.title>
        (file##filename |> ReasonReact.stringToElement)
      </h1>
      <div className=Styles.progressBarContainer>
        <div
          className=(
            Styles.progressBar(
              ~progress,
              ~status=Firebase.Model.statusFromJs(file##status),
            )
          )
        />
        <div className=Styles.progress>
          (
            Js.Float.toFixedWithPrecision(progress, ~digits=2)
            |> ReasonReact.stringToElement
          )
          (ReasonReact.stringToElement("%"))
        </div>
      </div>
    </div>;
  },
};