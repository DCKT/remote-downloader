let component = ReasonReact.statelessComponent("Loading");

let make = _children => {
  ...component,
  render: _self => <div> (ReasonReact.stringToElement("LOADING")) </div>,
};