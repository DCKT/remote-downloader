let component = "Login" |> ReasonReact.statelessComponent;

module Styles = {
  open Css;
  let root =
    style([
      display(`flex),
      justifyContent(center),
      alignItems(center),
      background(Colors.pageBackground),
      height(pct(100.)),
    ]);
  let box =
    style([
      background(hex("fff")),
      width(px(400)),
      padding(px(40)),
      boxShadow(~x=px(0), ~y=px(0), ~blur=px(12), rgba(24, 24, 24, 0.12)),
    ]);
  let form = style([marginTop(px(40))]);
  let title =
    style([
      fontSize(px(28)),
      fontWeight(700),
      textAlign(center),
      color(Colors.grey),
    ]);
  let inputContainer =
    style([display(block), marginBottom(px(15)), textAlign(center)]);
  let inputStyle = style([minWidth(px(220))]);
  let loader =
    style([
      position(absolute),
      left(px(15)),
      top(pct(50.)),
      marginTop(px(-10)),
    ]);
  let submitButton =
    style([
      position(relative),
      minWidth(px(220)),
      margin4(~top=px(20), ~right=auto, ~left=auto, ~bottom=px(0)),
    ]);
};

let make = (~isLogged: bool, ~connectUser, _children) => {
  ...component,
  didMount: _self => {
    if (isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Admin));
    };
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div className=Styles.root>
      <div className=Styles.box>
        <h1 className=Styles.title>
          (ReasonReact.stringToElement("Welcome"))
        </h1>
        <LoginForm.FormContainer
          initialState={email: "", password: ""}
          onSubmit=(
            (state, notify) => {
              Firebase.login(~email=state.email, ~password=state.password)
              |> Js.Promise.then_(() => {
                   notify.onSuccess();
                   connectUser();
                   Js.Promise.resolve();
                 })
              |> Js.Promise.catch(err => {
                   Js.log(err);
                   notify.onFailure();
                   Js.Promise.resolve();
                 })
              |> ignore;
              ();
            }
          )>
          ...(
               form =>
                 <form
                   className=Styles.form
                   onSubmit=(form.submit |> Formality.Dom.preventDefault)>
                   <Input
                     value=form.state.email
                     _type=`Email
                     placeholder="Email"
                     disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                     containerClassName=Styles.inputContainer
                     inputClassName=Styles.inputStyle
                     onChange=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnChange
                         |> form.change(LoginForm.Form.Email)
                     )
                     onBlur=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnBlur
                         |> form.blur(LoginForm.Form.Email)
                     )
                     error=(
                       switch (LoginForm.Form.Email |> form.results) {
                       | Some(Invalid(message)) =>
                         message |> ReasonReact.stringToElement
                       | Some(Valid)
                       | None => ReasonReact.nullElement
                       }
                     )
                   />
                   <Input
                     value=form.state.password
                     _type=`Password
                     placeholder="**********"
                     containerClassName=Styles.inputContainer
                     inputClassName=Styles.inputStyle
                     disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                     onChange=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnChange
                         |> form.change(LoginForm.Form.Password)
                     )
                     onBlur=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnBlur
                         |> form.blur(LoginForm.Form.Password)
                     )
                     error=(
                       switch (LoginForm.Form.Password |> form.results) {
                       | Some(Invalid(message)) =>
                         <div> (message |> ReasonReact.stringToElement) </div>
                       | Some(Valid)
                       | None => ReasonReact.nullElement
                       }
                     )
                   />
                   <Button
                     _type=`Submit
                     disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                     className=Styles.submitButton
                     content={
                       <Fragment>
                         (
                           form.submitting ?
                             <Loader
                               size=Loader.Tiny
                               isActive=true
                               color="#fff"
                               className=Styles.loader
                             /> :
                             ReasonReact.nullElement
                         )
                         (
                           (form.submitting ? "Submitting..." : "Login")
                           |> ReasonReact.stringToElement
                         )
                       </Fragment>
                     }
                   />
                 </form>
             )
        </LoginForm.FormContainer>
      </div>
    </div>,
  /* <form>
       <button onClick=connectUser>
         (ReasonReact.stringToElement("Connect me"))
       </button>
     </form> */
};