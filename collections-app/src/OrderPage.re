module D = Webapi.Dom;
module Doc = Webapi.Dom.Document;
module Elem = Webapi.Dom.Element;

type order = (int, ShirtSize.t);

/* Ok variantとしてorder型、Error variantとしてstringを返す */
type resultOrder = Belt.Result.t(order, string);

let commaSplit = (s: string): array(string) => {
  let pattern = [%re "/\\s*, \\s*/"];
  Js.String.splitByRe(pattern, s);
};

let orderFromCaptures =
    (optCaptures: option(array(string)), input: string): resultOrder =>
  switch (optCaptures) {
  | Some(captures) =>
    switch (ShirtSize.fromString(captures[2])) {
    | Some(size) => Belt.Result.Ok((int_of_string(captures[1]), size))
    | None => Belt.Result.Error(input)
    }
  | None => Belt.Result.Error(input)
  };

let toOrder = (input: string): resultOrder => {
  let pattern = [%re "/(\\d{1,4})\\s*@?\\s*(S|M|X{0,4}L)/"];
  Js.String.toUpperCase(input)
  ->Js.String.match(pattern, _)
  ->orderFromCaptures(input);
};

let orderPrice = ((n, size): order): float =>
  float_of_int(n) *. ShirtSize.price(size);

let addOrderTotal = ((totalShirts, totalPrice) as current, orderResult) =>
  switch (orderResult) {
  | Belt.Result.Ok((n, _) as order) => (
      totalShirts + n,
      totalPrice +. orderPrice(order),
    )
  | _ => current
  };

let calculateTotals = (orders: array(resultOrder)): (int, float) =>
  Belt.Array.reduce(orders, (0, 0.0), addOrderTotal);

/* Create a row from an order */
let createRow = (anOrder: resultOrder): string =>
  switch (anOrder) {
  | Belt.Result.Ok((n, size)) =>
    let totalPrice =
      Js.Float.toFixedWithPrecision(orderPrice((n, size)), ~digits=2);
    let sizeStr = ShirtSize.toString(size);
    {j|<tr>
      <td class="right">$n</td>
      <td class="center">$sizeStr</td>
      <td class="right">\$$totalPrice</td>
      </tr>\n|j};
  | Belt.Result.Error(s) => {j|<tr><td colspan="3">Bad Input $s</td></tr>\n|j}
  };

let createTable = (orderArray: array(resultOrder)): string => {
  let tableBody =
    Belt.Array.reduce(orderArray, "", (acc, item) => acc ++ createRow(item));

  {j|<table>
    <thead><tr><th>Quantity</th><th>Size</th><th>Price</th></thead>
    <tbody>|j}
  ++ tableBody
  ++ {j|
    </tbody>
    </table
    |j};
};

/* UI */
let getValue = (element: option(Elem.t)): option(string) =>
  element
  ->Belt.Option.map(_, Elem.unsafeAsHtmlElement)
  ->Belt.Option.map(_, D.HtmlElement.value);

let calcButton = Doc.getElementById("calculate", D.document);

let setInnerHTML = (id: string, htmlString: string) =>
  Doc.getElementById(id, D.document)
  ->Belt.Option.map(__x => Elem.setInnerHTML(__x, htmlString));

let setInnerText = (id: string, textString: string) =>
  Doc.getElementById(id, D.document)
  ->Belt.Option.map(__x => Elem.setInnerText(__x, textString));

let calculate = (_: Dom.event): unit =>
  switch (getValue(Doc.getElementById("orders", D.document))) {
  | Some(str) =>
    let orderArray =
      commaSplit(str)
      ->Belt.Array.keep(_, item => item != "")
      ->Belt.Array.map(_, toOrder);

    let (nShirts, grandTotal) = calculateTotals(orderArray);
    let priceString = Js.Float.toFixedWithPrecision(grandTotal, ~digits=2);
    let _ = setInnerHTML("table", createTable(orderArray));
    let _ = setInnerText("totalShirts", string_of_int(nShirts));
    let _ = setInnerText("totalPrice", priceString);
    ();
  | None => ()
  };

switch (calcButton) {
| Some(element) =>
  D.EventTarget.addEventListener(
    "click",
    calculate,
    D.Element.asEventTarget(element),
  )
| None => ()
};