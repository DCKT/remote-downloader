module Form = {
  type field =
    | Email
    | Password;
  type value = string;
  type state = {
    email: string,
    password: string,
  };
  type message = string;
  let get = (field, state) =>
    switch (field) {
    | Email => state.email
    | Password => state.password
    };
  let update = ((field, value), state) =>
    switch (field, value) {
    | (Email, value) => {...state, email: value}
    | (Password, value) => {...state, password: value}
    };
  let valueEmpty = value => value === "";
  let strategy = Formality.Strategy.OnFirstSuccessOrFirstBlur;
  module Validators =
    Formality.MakeValidators(
      {
        type t = field;
      },
    );
  type validators =
    Validators.t(Formality.validator(field, value, state, message));
  let validators =
    Formality.(
      Validators.empty
      |> Validators.add(
           Email,
           {
             strategy,
             dependents: None,
             validate: (value, _state) =>
               switch (value) {
               | "" => Invalid("This field is required")
               | _ => Valid
               },
           },
         )
      |> Validators.add(
           Password,
           {
             strategy,
             dependents: None,
             validate: (value, _state) =>
               switch (value) {
               | "" => Invalid("This field is required")
               | _ => Valid
               },
           },
         )
    );
};

module FormContainer = Formality.Make(Form);