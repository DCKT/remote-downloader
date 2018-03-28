type state = {files: list(Firebase.Model.file)};

type action =
  | UpdateFilesList(list(Firebase.Model.file));

let component = "Admin" |> ReasonReact.reducerComponent;

module Styles = {
  open Css;
  let root = style([background(hex("F7F8FB")), height(pct(100.))]);
  let pageContent =
    style([maxWidth(px(550)), margin2(~v=px(40), ~h=auto)]);
};

let make = (~isLogged: bool, _children) => {
  ...component,
  initialState: () => {files: []},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateFilesList(filesList) => ReasonReact.Update({files: filesList})
    },
  didMount: _self => {
    if (! isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Login));
    };
    ReasonReact.NoUpdate;
  },
  render: self =>
    <div className=Styles.root>
      <Navbar />
      <div className=Styles.pageContent>
        (
          List.length(self.state.files) == 0 ?
            ReasonReact.stringToElement("No downloads") :
            ReasonReact.nullElement
        )
        (
          self.state.files
          |> List.map(_file => ReasonReact.stringToElement("Test"))
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    </div>,
};