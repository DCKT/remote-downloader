type state = {
  files: list(Firebase.Model.file),
  isModalVisible: bool,
};

type action =
  | UpdateFilesList(list(Firebase.Model.file))
  | ShowModal
  | HideModal;

let component = "Admin" |> ReasonReact.reducerComponent;

module Styles = {
  open Css;
  let root = style([background(hex("F7F8FB")), height(pct(100.))]);
  let pageContent =
    style([maxWidth(px(550)), margin2(~v=px(40), ~h=auto)]);
  let empty = style([fontSize(px(18)), textAlign(center)]);
};

let make = (~isLogged: bool, _children) => {
  ...component,
  initialState: () => {files: [], isModalVisible: false},
  reducer: (action, state) =>
    switch (action) {
    | UpdateFilesList(filesList) =>
      ReasonReact.Update({...state, files: filesList})
    | ShowModal => ReasonReact.Update({...state, isModalVisible: true})
    | HideModal => ReasonReact.Update({...state, isModalVisible: false})
    },
  didMount: _self => {
    if (! isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Login));
    };
    ReasonReact.NoUpdate;
  },
  render: self =>
    <div className=Styles.root>
      <Navbar showModal=(_event => self.send(ShowModal)) />
      <div className=Styles.pageContent>
        (
          List.length(self.state.files) == 0 ?
            <p className=Styles.empty>
              (ReasonReact.stringToElement("No downloads"))
            </p> :
            ReasonReact.nullElement
        )
        (
          self.state.files
          |> List.map(_file => ReasonReact.stringToElement("Test"))
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
      <Modal
        isVisible=self.state.isModalVisible
        onClose=(_ => self.send(HideModal))
      />
    </div>,
};