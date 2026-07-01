Friend Class TranslationFunctions

    Shared Function BitValue(ByVal BitNr As Integer) As Long
        'Gibt den Wert eines Bits als Zahl zurück
        BitValue = 2 ^ BitNr
    End Function

    Shared Function GetString(ByVal RawData As String, ByRef PositionCounter As Integer, ByVal Length As Integer) As String
        'Liest eine Teilstring aus dem Raw-Daten String und erhöht den Postion-Counter
        Dim StringBuffer As String

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter + Length) > RawData.Length Then Exit Function

        Try
            'Entsprechenden String lesen
            StringBuffer = RawData.Substring(PositionCounter, Length) 'Zeichen lesen
            If Asc(StringBuffer) = 0 Then StringBuffer = Chr(32) 'nicht vorhandene Zeichen mit Leerzeichen ersetzen

            PositionCounter = PositionCounter + Length 'Positions-Zeiger weitersetzen

        Catch ex As Exception
            'Call HandleException("GetTextString", ex)
            Return (" ")
        End Try

        Return StringBuffer 'Fertigen String zurückgeben

    End Function

    Shared Function GetTextString(ByVal RawData As String, ByRef PositionCounter As Integer, ByVal Length As Integer) As String
        'Liest eine Teilstring aus dem Raw-Daten String und erhöht den Postion-Counter
        'Konvertiert zwischen LC-Display und Windows-Zeichensatz
        Dim Character As String
        Dim StringBuffer As String

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter + Length) > RawData.Length Then Exit Function

        Try
            'Entsprechenden String lesen und in ASCII umwandeln
            For i As Integer = 1 To Length
                Character = RawData.Substring(PositionCounter, 1) 'Zeichen lesen
                If Character = "" Then Character = Chr(0) 'nicht vorhandene Zeichen mit Leerzeichen ersetzen

                'ASCII-Code des Gerätes in Windows-Code wandeln
                Select Case Asc(Character)
                    Case 0
                        Character = Chr(32)
                    Case 245 '"ü"
                        Character = Chr(252)
                    Case 225 '"ä"
                        Character = Chr(228)
                    Case 239 '"ö"
                        Character = Chr(246)
                    Case 226 '"ß"
                        Character = Chr(223)
                    Case 228 '"µ"
                        Character = Chr(181)
                End Select

                StringBuffer = StringBuffer & Character
                PositionCounter = PositionCounter + 1 'Positions-Zeiger weitersetzen
            Next

        Catch ex As Exception
            'Call HandleException("GetTextString", ex)
            Return (" ")
        End Try

        Return StringBuffer 'Fertigen String zurückgeben

    End Function

    Shared Function GetHNibble(ByVal RawData As String, ByRef PositionCounter As Integer) As Byte
        'Gibt ein Byte mit dem Nibble-Wert der oberen 4 bit zurück
        Dim Character As Byte
        Dim Buffer As Byte

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter) > RawData.Length Then Exit Function

        Character = Asc(RawData.Substring(PositionCounter, 1))

        Try
            For i As Integer = 4 To 8 Step (-1)
                If (Character And (2 ^ i)) = (2 ^ i) Then
                    Buffer = Buffer + (2 ^ (i - 4))
                End If
            Next

            PositionCounter = PositionCounter + 1 'Positions-Zeiger weitersetzen

        Catch ex As Exception
            'Call HandleException("GetHNibble", ex)
            Return (0)
        End Try

        Return Buffer

    End Function

    Shared Function GetLNibble(ByVal RawData As String, ByRef PositionCounter As Integer) As Byte
        'Gibt ein Byte mit dem Nibble-Wert der unteren 4 bit zurück

        Dim Character As Byte
        Dim Buffer As Byte

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter) > RawData.Length Then Exit Function

        Character = Asc(RawData.Substring(PositionCounter, 1))

        Try
            For i As Integer = 1 To 4 Step (-1)
                If (Character And (2 ^ i)) = (2 ^ i) Then
                    Buffer = Buffer + (2 ^ i)
                End If
            Next

        Catch ex As Exception
            'Call HandleException("GetLNibble", ex)
            Return (0)
        End Try

        Return Buffer

    End Function

    Shared Function GetByte(ByVal RawData As String, ByRef PositionCounter As Integer) As Byte

        Dim Buffer As Byte

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter) > RawData.Length Then Exit Function

        Try
            Buffer = Asc(RawData.Substring(PositionCounter, 1))
            PositionCounter = PositionCounter + 1 'Um 1 Byte weitersetzen

        Catch ex As Exception
            'Call HandleException("GetByte", ex)
            Return (0)
        End Try

        Return Buffer

    End Function

    Shared Function GetInteger(ByVal RawData As String, ByRef PositionCounter As Integer) As Integer

        Dim Buffer As Integer

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter + 1) > RawData.Length Then Exit Function

        Try
            Buffer = (Asc(RawData.Substring(PositionCounter, 1)) * 256) + Asc(RawData.Substring(PositionCounter + 1, 1))
            PositionCounter = PositionCounter + 2 'Um 2 Byte weitersetzen

        Catch ex As Exception
            'Call HandleException("GetInteger", ex)
            Return (0)
        End Try

        Return Buffer

    End Function

    Shared Function GetLong(ByVal RawData As String, ByRef PositionCounter As Integer) As Long

        Dim Buffer As Long

        'Abbruch, wenn Länge des Strings überschritten würde
        If (PositionCounter + 3) > RawData.Length Then Exit Function

        Try
            Buffer = Asc(RawData.Substring(PositionCounter, 1)) * (2 ^ 24) + _
                     Asc(RawData.Substring(PositionCounter + 1, 1)) * (2 ^ 16) + _
                     Asc(RawData.Substring(PositionCounter + 2, 1)) * (2 ^ 8) + _
                     Asc(RawData.Substring(PositionCounter + 3, 1))

            PositionCounter = PositionCounter + 4 'Um 4 Byte weitersetzen
            Return Buffer

        Catch ex As Exception
            'Call HandleException("GetLong", ex)
            Return (0)
        End Try

    End Function

    Shared Sub AddNibbles(ByVal HNibble As Byte, ByVal LNibble As Byte, ByRef RawData As String)
        'Fügt ein Byte am Ende des Strings aus zwei Nibbles zusammen
        Try
            RawData += Chr((HNibble * 16) + LNibble)
        Catch ex As Exception
            Call HandleException("AddByte", ex)
        End Try
    End Sub

    Shared Sub AddByte(ByVal myByte As Byte, ByRef RawData As String)
        'Fügt ein Byte am Ende des Strings an
        Try
            RawData += Chr(myByte)
        Catch ex As Exception
            Call HandleException("AddByte", ex)
        End Try
    End Sub

    Shared Sub AddInteger(ByVal myInteger As Integer, ByRef RawData As String)
        'Fügt ein Byte am Ende des Strings an
        Dim a As Integer
        Dim b As Integer

        Try

            a = Int(myInteger \ (2 ^ 8))
            b = myInteger - (a * (2 ^ 8))

            RawData += Chr(a)
            RawData += Chr(b)

        Catch ex As Exception
            Call HandleException("AddInteger", ex)
        End Try
    End Sub

    Shared Sub AddLong(ByVal myLong As Long, ByRef RawData As String)
        'Fügt ein Byte am Ende des Strings an
        Dim a As Integer
        Dim b As Integer
        Dim c As Integer
        Dim d As Integer

        Dim Rest As Long

        Try
            a = myLong \ (2 ^ 24)
            myLong = myLong - (a * (2 ^ 24))

            b = myLong \ (2 ^ 16)
            myLong = myLong - (b * (2 ^ 16))

            c = myLong \ (2 ^ 8)
            d = myLong - (c * (2 ^ 8))

            RawData += Chr(a)
            RawData += Chr(b)
            RawData += Chr(c)
            RawData += Chr(d)

        Catch ex As Exception
            Call HandleException("AddLong", ex)
        End Try
    End Sub

    Shared Sub AddTextString(ByVal myString As String, ByVal Length As Integer, ByRef RawData As String)

        Dim Character As String

        'Fügt ein Zeichen am Ende des Strings an
        'Konvertiert zwischen Windows-ANSI-Code und LC-Display
        Try

            For i As Integer = 0 To (Length - 1)

                If i > (Len(myString) - 1) Then
                    Character = Chr(0)
                Else
                    Character = myString.Substring(i, 1) 'Zeichen lesen

                    'Windows ASCII-Code in Geräte-Code wandeln
                    Select Case Asc(Character)
                        Case 252, 220 '"ü, Ü"
                            Character = Chr(245)
                        Case 196, 192, 193 '"Ä, À, Á"
                            Character = Chr(225)
                        Case 228, 196, 192, 193 '"ä, Ä, À, Á"
                            Character = Chr(225)
                        Case 201, 200, 202 '"Ê, Ë, È"
                            Character = Chr(69)
                        Case 237, 236 '"í, ì"
                            Character = Chr(105)
                        Case 204, 205 '"Ì, Í"
                            Character = Chr(73)
                        Case 246, 214 '"ö, Ö"
                            Character = Chr(239)
                        Case 243, 242 '"ó, ò"
                            Character = Chr(111)
                        Case 223 '"ß"
                            Character = Chr(226)
                        Case 233, 232 '"é", "è"
                            Character = Chr(101)
                        Case 181 '"µ"
                            Character = Chr(228)
                    End Select
                End If
                RawData += Character
            Next

        Catch ex As Exception
            Call HandleException("AddTextString", ex)
        End Try
    End Sub

    Shared Sub AddString(ByVal myString As String, ByVal Length As Integer, ByRef RawData As String)

        Dim Character As String

        'Fügt ein Zeichen am Ende des Strings an

        Try

            For i As Integer = 0 To (Length - 1)

                If i > (Len(myString) - 1) Then
                    Character = Chr(0)
                Else
                    Character = myString.Substring(i, 1) 'Zeichen lesen
                End If
                RawData += Character
            Next

        Catch ex As Exception
            Call HandleException("AddString", ex)
        End Try
    End Sub

    Shared Sub HandleException(ByVal moduleName As String, ByVal e As Exception)

        'Purpose    : Provides a central mechanism for exception handling.
        '           : Displays a message box that describes the exception.

        'Accepts    : moduleName - the module where the exception occurred.
        '           : e - the exception

        Dim Message As String
        Dim Caption As String

        Try
            'Create an error message.
            Message = "Exception: " & e.Message & ControlChars.CrLf & _
            "Module: " & moduleName & ControlChars.CrLf & _
            "Method: " & e.TargetSite.Name

            'Specify a caption.
            Caption = "Unexpected Exception"

            'Display the message in a message box.
            MessageBox.Show(Message, Caption, MessageBoxButtons.OK)
            Debug.Write(Message)
        Finally
        End Try
    End Sub

End Class
