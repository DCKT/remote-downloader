module Styles = {
  open Css;
  let root =
    style([
      background(Colors.white),
      borderRadius(px(2)),
      boxShadow(~x=px(0), ~y=px(0), ~blur=px(12), rgba(24, 24, 24, 0.12)),
      padding(px(30)),
      position(relative),
      selector(":not(:last-child)", [marginBottom(px(30))]),
    ]);
  let title =
    style([
      fontSize(px(16)),
      color(Colors.black),
      textTransform(uppercase),
      marginBottom(px(15)),
      fontWeight(`bold),
    ]);
  let progressBarContainer = style([display(`flex), alignItems(center)]);
  let progressBar = (~progress: float, ~status: option(File.status)) => {
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
      after([
        unsafe("content", "' '"),
        display(`block),
        width(pct(100.)),
        height(pct(100.)),
        background(barColor),
        borderRadius(px(2)),
        width(pct(progress)),
        transition(~duration=1000, ~timingFunction=`ease, "all"),
      ]),
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

[@react.component]
let make = (~file: File.t) => {
  let progress =
    float_of_int(file->File.progressGet)
    /. float_of_int(file->File.sizeGet)
    *. 100.;

  Js.log(progress);
  <div className=Styles.root>
    <button
      className=Styles.deleteButton
      onClick={_event => File.remove(~id=file->File.idGet)}>
      <i className={"material-icons " ++ Styles.deleteIcon}>
        {React.string("delete_forever")}
      </i>
    </button>
    <h1 className=Styles.title> file->File.filenameGet->React.string </h1>
    <div className=Styles.progressBarContainer>
      <div
        className={
          Styles.progressBar(
            ~progress,
            ~status=File.statusFromJs(file->File.statusGet),
          )
        }
      />
      <div className=Styles.progress>
        {
          progress->Js.Float.isNaN ?
            "0" |> React.string :
            Js.Float.toFixedWithPrecision(progress, ~digits=2) |> React.string
        }
        {React.string("%")}
      </div>
    </div>
  </div>;
};