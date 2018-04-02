type state = {
  files: array(Firebase.Model.file),
  isModalVisible: bool,
};

type action =
  | UpdateFiles(array(Firebase.Model.file))
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
  initialState: () => {files: [||], isModalVisible: false},
  reducer: (action, state) =>
    switch (action) {
    | UpdateFiles(files) => ReasonReact.Update({...state, files})
    | ShowModal => ReasonReact.Update({...state, isModalVisible: true})
    | HideModal => ReasonReact.Update({...state, isModalVisible: false})
    },
  didMount: self => {
    if (! isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Login));
    };
    let db = Firebase.App.database(Firebase.app);

    Firebase.Database.on(Firebase.Database.ref(db, "files"), ~eventType="value", ~callback=snapshot => {
      let data = Firebase.DataSnapshot.val_(snapshot);
      let value = Lodash.map(data);
      self.send(UpdateFiles(value));
      ();
    }) |> ignore;
    ReasonReact.NoUpdate;
  },
  render: self => 
    <div className=Styles.root>
      <Navbar showModal=(_event => self.send(ShowModal)) />
      <div className=Styles.pageContent>
        (
          Array.length(self.state.files) == 0 ?
            <p className=Styles.empty>
              (ReasonReact.stringToElement("No downloads"))
            </p> :
            ReasonReact.nullElement
        )
        (
          self.state.files
          |> Array.mapi((index, file) => {
            <DownloadItem key=(string_of_int(index)) file />
          })
          |> ReasonReact.arrayToElement
        )
      </div>
      <Modal
        isVisible=self.state.isModalVisible
        onClose=(_ => self.send(HideModal))
      />
    </div>,
};