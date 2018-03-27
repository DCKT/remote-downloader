type state = {
  isSync: bool,
  isConnected: bool,
};

type action =
  | ConnectUser
  | DisconnectUser
  | SyncApp;

let component = "Root" |> ReasonReact.reducerComponent;

let make = _children => {
  ...component,
  initialState: () => {isConnected: false, isSync: false},
  didMount: self => {
    Firebase.Authentication.onAuthStateChanged(Firebase.authInstance, user => {
      switch (Js.Nullable.toOption(user)) {
      | None => self.send(SyncApp);
      | Some(u) => self.send(ConnectUser);
      };
    });
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | SyncApp => ReasonReact.Update({ ...state, isSync: true })
    | ConnectUser =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isConnected: true, isSync: true},
        (_self => ReasonReact.Router.push("/")),
      )
    | DisconnectUser => ReasonReact.Update({ isSync: true, isConnected: false})
    },
  render: (self) => {
    let isLogged = self.state.isConnected;
    <Router>
      ...(
        (~currentRoute) => {
          switch (self.state.isSync) {
            | false => <Loading />
            | true => switch (currentRoute) {
              | Admin => <Admin isLogged />
              | Login => <Login isLogged connectUser=(_ => self.send(ConnectUser)) />
              | NotFound => <NotFound />
            }
          }
        }
      )
    </Router>
  }
};