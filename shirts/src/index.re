module D = Webapi.Dom;
module Doc = Webapi.Dom.Document;
module Elem = Webapi.Dom.Element;

type shirtSize =
  | Small
  | Medium
  | Large
  | XLarge(int);

let toFixied2 = Js.Float.toFixedWithPrecision(~digits=2);

let calcButton = Doc.getElementById("calculate", D.document);

let shirtSizeOfString = (str: string): option(shirtSize) =>
  switch (str) {
  | "S" => Some(Small)
  | "M" => Some(Medium)
  | "L" => Some(Large)
  | "XL" => Some(XLarge(1))
  | "XXL" => Some(XLarge(2))
  | "XXXL" => Some(XLarge(3))
  | _ => None
  };

let price = (size: shirtSize): float =>
  switch (size) {
  | Small => 11.00
  | Medium => 12.50
  | Large => 14.00
  | XLarge(n) => 16.00 +. float_of_int(n - 1) *. 0.50
  };

let toInt = (s: string): option(int) =>
  switch (int_of_string(s)) {
  | result => Some(result)
  | exception (Failure("int_of_string")) => None
  };

let getValue = (element: option(Elem.t)): option(string) =>
  element
  ->Belt.Option.map(_, Elem.unsafeAsHtmlElement)
  ->Belt.Option.map(_, D.HtmlElement.value);

let calculate = (_: Dom.event): unit => {
  let quantity =
    getValue(Doc.getElementById("quantity", D.document))
    ->Belt.Option.flatMap(_, toInt);

  let unitPrice =
    getValue(Doc.getElementById("size", D.document))
    ->Belt.Option.flatMap(_, shirtSizeOfString)
    ->Belt.Option.map(_, price);

  let totalPrice =
    switch (unitPrice, quantity) {
    | (Some(uPrice), Some(qty)) => Some(uPrice *. float_of_int(qty))
    | (_, _) => None
    };

  let priceString =
    switch (totalPrice) {
    | Some(total) => toFixied2(total)
    | None => ""
    };

  let _ =
    Doc.getElementById("price", D.document)
    ->Belt.Option.map(_, Elem.setInnerText(_, priceString));

  (); /* return unit */
};

switch (calcButton) {
| Some(element) =>
  D.EventTarget.addEventListener(
    "click",
    calculate,
    Elem.asEventTarget(element),
  )
| None => ()
};