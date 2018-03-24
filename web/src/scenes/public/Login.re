let component = "Login" |> ReasonReact.statelessComponent;

let make = _children => {
  ...component,
  render: _self =>
    <div> <h1> (ReasonReact.stringToElement("Login")) </h1> </div>,
};