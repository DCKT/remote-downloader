let component = ReasonReact.statelessComponent("Admin");

let make = (~isLogged: bool, _children) => {
  ...component,
  didMount: _self => {
    if (! isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Login));
    };
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div> <h1> (ReasonReact.stringToElement("Admin")) </h1> </div>,
};