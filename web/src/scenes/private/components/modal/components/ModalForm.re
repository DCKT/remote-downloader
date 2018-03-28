module Form = {
  type field =
    | Link;
  type value = string;
  type state = {link: string};
  type message = string;
  let get = (field, state) =>
    switch (field) {
    | Link => state.link
    };
  let update = ((field, value), _state) =>
    switch (field, value) {
    | (Link, value) => {link: value}
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
           Link,
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