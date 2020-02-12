[@bs.val] external fetch: string => Js.Promise.t('a) = "fetch";

// FIXME: lol wtf parsing date is hard
let parseDate: string => string = [%raw
  {|
    function (wtfDate) {
      if (!wtfDate) return ''

      let $date = new Date(wtfDate)

      let dateString = $date.toDateString()
      let timeString = $date.toLocaleTimeString()

      return `${dateString} (${timeString})`
    }
  |}
];