PROGRAM CONVERT1;
{ Convert from Fahrenheit to Celsius! }
VAR
  F: REAL; { Fahrenheit }
  C: REAL; { Celsius }
BEGIN
  READ (F) ;
  IF F < -100 THEN
    BEGIN
  	  WRITE('Temp is too low!');
	    WRITE('Is it really that cold?')
    END
  ELSE
    BEGIN
      { Conversion formula }
      C := (F - 32.0)*(5.0/9.0);
      { Tell the user about it }
      WRITE (' FAHRENHEIT= ');
      WRITE (F);
      WRITE (' CELSIUS= ');
      WRITE (C)
    END
END
