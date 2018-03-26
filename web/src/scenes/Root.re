type state = {
  isSync: bool,
  isConnected: bool,
};

type action =
  | ConnectUser
  | DisconnectUser;

let component = "Root" |> ReasonReact.reducerComponent;

let make = _children => {
  ...component,
  initialState: () => {isConnected: false, isSync: false},
  didMount: _self =>
    ReasonReact.Update({
      isSync: true,
      isConnected:
        switch (Localstorage.getItem("isConnected")) {
        | None => false
        | Some(_value) => true
        },
    }),
  reducer: (action, state) =>
    switch (action) {
    | ConnectUser =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isConnected: true},
        (_self => ReasonReact.Router.push("/")),
      )
    | DisconnectUser => ReasonReact.Update({...state, isConnected: false})
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
              | Login=> <Login isLogged connectUser=(_ => self.send(ConnectUser)) />
              | NotFound => <NotFound />
            }
          }
        }
      )
    </Router>
  }
};