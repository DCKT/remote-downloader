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
      fontWeight(`bold),
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

module LoginForm = {
  open Formality;

  type field =
    | Email
    | Password;

  type state = {
    email: string,
    password: string,
  };

  type message = string;
  type submissionError = unit;

  module EmailField = {
    let update = (state, value) => {...state, email: value};

    let validator = {
      field: Email,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({email}) => {
        let emailRegex = [%bs.re {|/.*@.*\..+/|}];
        switch (email) {
        | "" => Error("Email is required")
        | _ as value when !emailRegex->Js.Re.test_(value) =>
          Error("Email is invalid")
        | _ => Ok(Valid)
        };
      },
    };
  };

  module PasswordField = {
    let update = (state, value) => {...state, password: value};

    let validator = {
      field: Password,
      strategy: Strategy.OnFirstBlur,
      dependents: None,
      validate: ({password}) =>
        switch (password) {
        | "" => Error("Password is required")
        | _ => Ok(Valid)
        },
    };
  };

  let validators = [EmailField.validator, PasswordField.validator];
};

module LoginFormHook = Formality.Make(LoginForm);

let initialState = LoginForm.{email: "", password: ""};

[@react.component]
let make = (~onLoginSuccess) => {
  let form =
    LoginFormHook.useForm(
      ~initialState,
      ~onSubmit=(state, form) => {
        Firebase.login(~email=state.email, ~password=state.password)
        |> Js.Promise.then_(() => {
             form.notifyOnSuccess(None);
             onLoginSuccess();
             Js.Promise.resolve();
           })
        |> Js.Promise.catch(err => {
             Js.log(err);
             form.notifyOnFailure();
             Js.Promise.resolve();
           })
        |> ignore;
        ();
      },
    );

  <div className=Styles.root>
    <div className=Styles.box>
      <h1 className=Styles.title> "Welcome"->React.string </h1>
      <form
        className=Styles.form
        onSubmit=form.submit->Formality.Dom.preventDefault>
        <Input
          value={form.state.email}
          _type=`Email
          placeholder="Email"
          disabled={form.submitting}
          containerClassName=Styles.inputContainer
          inputClassName=Styles.inputStyle
          onChange={
            event =>
              form.change(
                Email,
                LoginForm.EmailField.update(
                  form.state,
                  event->ReactEvent.Form.target##value,
                ),
              )
          }
          onBlur={_ => form.blur(Email)}
          error={
            switch (Email->(form.result)) {
            | Some(Error(message)) => message->React.string
            | Some(Ok(Valid))
            | Some(Ok(NoValue))
            | None => React.null
            }
          }
        />
        <Input
          value={form.state.password}
          _type=`Password
          placeholder="**********"
          containerClassName=Styles.inputContainer
          inputClassName=Styles.inputStyle
          disabled={form.submitting}
          onChange={
            event =>
              form.change(
                Password,
                LoginForm.PasswordField.update(
                  form.state,
                  event->ReactEvent.Form.target##value,
                ),
              )
          }
          onBlur={_ => form.blur(Password)}
          error={
            switch (Password->(form.result)) {
            | Some(Error(message)) => message->React.string
            | Some(Ok(Valid))
            | Some(Ok(NoValue))
            | None => React.null
            }
          }
        />
        <Button
          _type=`Submit
          disabled={form.submitting}
          className=Styles.submitButton
          content=
            <>
              {
                form.submitting ?
                  <Loader
                    size=Loader.Tiny
                    isActive=true
                    color="#fff"
                    className=Styles.loader
                  /> :
                  React.null
              }
              {(form.submitting ? "Submitting..." : "Login") |> React.string}
            </>
        />
        {
          switch (form.status) {
          | SubmissionFailed(_) => <p> "An error occured"->React.string </p>
          | _ => React.null
          }
        }
      </form>
    </div>
  </div>;
};