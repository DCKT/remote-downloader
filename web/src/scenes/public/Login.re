let component = "Login" |> ReasonReact.statelessComponent;

let make = (~isLogged: bool, ~connectUser, _children) => {
  ...component,
  didMount: _self => {
    if (isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Admin));
    };
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div>
      <h1> (ReasonReact.stringToElement("Login")) </h1>
      <button onClick=connectUser>
        (ReasonReact.stringToElement("Connect me"))
      </button>
    </div>,
};