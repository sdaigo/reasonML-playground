let orderedList = [
  (7, ShirtSize.Medium),
  (5, ShirtSize.XLarge(3)),
  (4, ShirtSize.Small),
  (6, ShirtSize.Large),
  (8, ShirtSize.Small),
  (2, ShirtSize.Large),
  (9, ShirtSize.Medium),
  (3, ShirtSize.XLarge(2)),
];

let onePrice = ((qty: int, size: ShirtSize.t)): float =>
  float_of_int(qty) *. ShirtSize.price(size);

let isMedium = ((_, size: ShirtSize.t)): bool => size === ShirtSize.Medium;

/* map */
let priceList = Belt.List.map(orderedList, onePrice);
Js.log(priceList);

/* keep */
let mediums = Belt.List.keep(orderedList, isMedium);
let mediums2 =
  Belt.List.keep(orderedList, ((_, size)) => size === ShirtSize.Medium);
Js.log(mediums);
Js.log(mediums2);

/* reduce */
let addPrice = (runningTotal, orderItem) =>
  runningTotal +. onePrice(orderItem);

let totalPrice = Belt.List.reduce(orderedList, 0.0, addPrice);
Js.log2("total price: ", totalPrice);

let addPriceToTotal = (runningTotal, price) => runningTotal +. price;
let totalPrice2 =
  Belt.List.map(orderedList, onePrice)
  ->Belt.List.reduce(_, 0.0, addPriceToTotal);
Js.log2("total price: ", totalPrice2);

let mediumTotal =
  Belt.List.keep(orderedList, isMedium)
  ->Belt.List.map(_, onePrice)
  ->Belt.List.reduce(_, 0.0, addPriceToTotal);
Js.log2("Medium total: ", mediumTotal);