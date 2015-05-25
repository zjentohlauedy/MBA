module Positions
  None        =  0
  Catcher     =  1
  FirstBase   =  2
  SecondBase  =  3
  ThirdBase   =  4
  ShortStop   =  5
  LeftField   =  6
  CenterField =  7
  RightField  =  8
  Infield     =  9
  Outfield    = 10

  def self.string_value position
    case position
    when None;        return 'N/A'
    when Catcher;     return 'C'
    when FirstBase;   return '1B'
    when SecondBase;  return '2B'
    when ThirdBase;   return '3B'
    when ShortStop;   return 'SS'
    when LeftField;   return 'LF'
    when CenterField; return 'CF'
    when RightField;  return 'RF'
    when Infield;     return 'IF'
    when Outfield;    return 'OF'
    end
  end
end
