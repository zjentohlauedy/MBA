module Positions
  None          =  0
  Catcher       =  1
  FirstBaseman  =  2
  SecondBaseman =  3
  ThirdBaseman  =  4
  ShortStop     =  5
  LeftFielder   =  6
  CenterFielder =  7
  RightFielder  =  8
  Infielder     =  9
  Outfielder    = 10


  def self.string_value position
    case position
    when None;          return 'N/A'
    when Catcher;       return 'C'
    when FirstBaseman;  return '1B'
    when SecondBaseman; return '2B'
    when ThirdBaseman;  return '3B'
    when ShortStop;     return 'SS'
    when LeftFielder;   return 'LF'
    when CenterFielder; return 'CF'
    when RightFielder;  return 'RF'
    when Infielder;     return 'IF'
    when Outfielder;    return 'OF'
    end
  end
end
