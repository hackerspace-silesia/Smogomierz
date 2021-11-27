#ifdef ARDUINO_ARCH_ESP8266
#include <avr/pgmspace.h>
#endif

static const char WEB_PAGE_HEADER_CSS[] PROGMEM = "<link rel='stylesheet' href='https://rawcdn.githack.com/hackerspace-silesia/Smogomierz/b1e4dd9a2b6fddb63da88c46ef934509ab1def44/css/smogly.css'>";
// via http://raw.githack.com 
//static const char WEB_PAGE_HEADER_CSS[] PROGMEM = "<link rel='stylesheet' href='https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/css/smogly.css'>";
// https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/dev/src/css/smogly.css
// based on bootstrap.min.css - 4.4.1 and Bootswatch v4.4.1
// https://raw.githack.com

#ifdef ARDUINO_ARCH_ESP8266
const char WEB_PAGE_HEADER[] PROGMEM = R"rawliteral(<html lang='{Language}'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - {CurrentPageTitle}</title>
{WEB_PAGE_CSS}
</head><body>
	<nav class='navbar navbar-expand-lg navbar-dark bg-dark' style='padding-left:15%'>
	<a class='navbar-brand' href='/'>Smogly</a>
	  <button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarColor02' aria-controls='navbarColor02' aria-expanded='false' aria-label='Toggle navigation'>
	  <span class='navbar-toggler-icon'></span>
	  </button>
	  <div class='collapse navbar-collapse' id='navbarColor02' style='padding-left:2rem'>
	    <ul class='navbar-nav mr-auto'>
	      <li class='nav-item'>
	        <a class='nav-link' href='/'>{IndexPageTitle}</a>
	      </li>
	      <li class='nav-item'>
	        <a class='nav-link' href='/config'>{ConfigPageTitle}</a>
	      </li>
		  <li class='nav-item'>
		  	<a class='nav-link' href='/update'>{UpdatePageTitle}</a>
		  </li>
	    </ul>
</div></nav>
		)rawliteral";

static const char SMOGLY_LOGO_URL[] PROGMEM = "<a class='navbar-brand' href='/'>Smogly</a>";

#elif defined ARDUINO_ARCH_ESP32
static const char WEB_PAGE_HEADER[] PROGMEM = R"rawliteral(<html lang='{Language}'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - {CurrentPageTitle}</title>
{WEB_PAGE_CSS}
</head><body>
	<nav class='navbar navbar-expand-lg navbar-dark bg-dark' style='padding-left:15%'>
	<a class='navbar-brand' href='/'><img border='0' width='165' height='40' alt='SmoglyLogo' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAk4AAACPCAMAAAA7rKT/AAAABGdBTUEAALGPC/xhBQAAAAFzUkdCAK7OHOkAAAL9UExURUdwTJ2dmldXV6GhoXZ2dmlpaWRkZFFRUVNTU1JSUmRkZGhoaIGBgVZWVnl5eVhYWGhoaFxcXOXrxeHspsDQatbikOfn54GBgVJSUmlpaY+Pj+bxrLfKWajBI6rEHarEJ67GPrvPXrbLVc7bhlxcXGBgYKnFDqnFD6nFEanEEanFDKjDGcPVcsfWdnR0dKnEFanFEKnGDarGEKnGC6nEE63FNrbJVM/Tt1ZWVlhYWKnFErLIS3JycrvOYdPgk2ZmZn5+frzOY9rmoOTnyMvOwHBwcNDggufwt6nFE8fXdaK6H5y2EqG7EabCEc3ch8TUd8bYccrZeb7HkK64eJOjQ56vTpWrIdjbvlVVVc/diKjEBrbLU7zPYObvtqSyW5elVoudM46mC6W/FcDRa8vZg5KrD7bKVcHTcomdIrDHRZqtPZevEnZ2dsLTcHt7e5mvJml3HRwfDQ4OD1tnGZ2dnRYXDxcXGhcXFX+RIaSkpExMTEdRE0JCQE5OTu/w7/39/NPT1KysrKzFLm1ubYWFhSoqLVJSUiwyDq3EMq+vr8LCwn5+fri4uLa2tpOTk1RUVICAgGxsbHFxcV1dXXd3d3x8fI+Pj2tra4aGhnJycp6ennh4eGtra25uboyMjHZ2dp+fn39/f5mZmWhoaHR0dIKCgnNzc1xcXIuLi1xcXHh4eF5eXnx8fGlpaW5uboGBgZCQkGNjY7u7u7q8tHBwcFhYWH5+fmJiYlNTU4KCgsPDw4uLi21tbVxcXFVVVby8vL6+vrm5uVZWVoWFhXV1dWVlZbi4uLS0tLi4uWFhYY+QjG1tbVtbW5ubm2JiYoODg1VVVW2ABWN0B3WLCX19fWJiYmV4A1JSUnuQCIieC6GhoVhYWKioqFJSUpqamlNTU4OaCYODgpubmaOjpLPNJMPbSIyMjNjog2dnZ9zplsjYelFRUbnVH77ZNbTRG77aI77aJsjdXr7bIr3ZIq/LFVJSUmFhYZ6enr7bIJGRkdfmi7LJP6m1dX+VBZWhXKAUvi8AAAD/dFJOUwAWMCAdmfH/////qTACoP/RFBpbWlMBB8L/OzPk//////6xrcn/////////32uX/////////8MUCvH//m7Zb8T08IcMBfOdKf+z/////77HnzFvrcn1/wj/hP//yUSM2f3//79G//H5//XY//+A////////LP////9C/////////////v//rv/BNk5C/+H9Wq3n27e0iEeGmlgPgbRjr+phUJdMq//N/3PZwu5r87x+g+jJJaOFY9bkw0IlPeT7od3t9lx494T9cPZqxPlTb+yl////0Cf/u////3noQXPb/9vfx///yvnfrpHz///////3////mpx9/9/M19T/tvtpMuwAACYMSURBVHgB7NgFl9s4EMBxHSYuMycpakby7nlsuaRyl9ykzMxMSxd7r8z8oc9ezq7LEPs9/QL+Av83HokZP9hvvyf+YN/LMP786+9C0bKsYnHK1GnsOxjG9Bkzi0VrUPwozJrNcmLO3Hnz585ZwIwMWbioYI1TKCxewvJgwdJlpXKpsnzFylXzs9KUMX21lcym8dbkYz7NK3NAIYREe3lL61KWAcY/BWui4qx85ISOFMAJXE+Bv3Zd6/pmzyhjw8aiNdm0XORke9xxgYaAFptWzm9uUMZmK81fLAfml1zSQKOQy8qWreuZ0TzbrLTptJ3lwJxN6AGN4Yjo2Dt2tjGjSdo7CtZkxSksD9Y5QOMgClBA5WXz25nRFAvXWGkKLA86fSROo1zSkjRIWWkxx7zm2JBek8VyYHpXJfC0LwTSIBz+chKbdmVlhTI5JVgetK2o1mq13XuAJgBl75jPDJPTRG3z9u7bv/XAxGHT/tvBQ4ePHK3tsQNNkwB3vbWrjrFMMDkVj7OsOFFxSJ4snzp94kzbaEsHzp47f6F20Xe4FIiUwgdur8vIBmWm02KWFZdcLvxACxBrd3R2LUhiunzu/JWqHZCUIJWkNKgVAV49wAyT0zjHrrmcSLoelxz5pn1b2eVz12uaSwkEwFEgpUEpNCh14xf3ZHIqZDun9hU0Ch2XV25erwVCAH0JcExPZjqN134KaBTgngu3bKkAgL4AukRXM7A/mXunrOS0YHxOqC9euSgRiSSnL6C5EvaOY8ww0ynlZQd7ao6vJaFwAvoCwvekH1xqY01mptPtzKziK5BG7XG4CjSC4vRxACSU5BpIoS+Al++wJjPT6W52TnY0xvPos5B8Ua52d9sAHJHI42u3MsPkNKj9a3MiTqWe3r7e/qOOH6AiHQRX5zDj1/i3kO2c2Er6OoB2fz0Mo6h3wFbElZDo7WPGr/HfvYzntAvoK12vR0lOYdwT+Qp54FZOMMPklNgPlA4oFdr3wwe345rinrq5Dz7FPZ2aw34Jk9PDbOc0fZ6gRhIICUlK0JTC7e6d8ujxkzBxv4QqICByLjHD5BRrdahR4AYOonAcwSkFXO97+vjx4wdRrN6ttE8x8ewyM0xObMHcEjXivlRccVASKIU7ED1/HE+nKAzDejeRpoS7o401jclpFsuKtuXUiGu7fPT8Udt3RPp0qkczX7yMknW83yZHUUKWW1nTmJxesaw4dooaKd19P6r33r9e5pSm+jpZw+thWO+vepKGAJjLJ5NT47U4IgdEv/t1GEaxnouKSHNqJJ3zr6N6XNT966XAJUBKBMJZxZrFXBRkKKcWGoUARKX7URiLg+kvuTh5H/e86kBPz8DREndpBHDwrprt6edbkvXpxN7QGETOrw+mFCbfnpTTnfaVAkmkXE/QCAApn81nE719x4wfacH7Qn5edgQ+yXJ/9HJmmGza9ah+XSgfqQFw8n2MCYE0ArSWzn42wYJ3H0xPP1a+chKSqq/vxbeUgye3MLpfUpoaASACqASnEUAIuOx/Xs76u20fCODlk9Mq+aI75tkZhl/fmPH38b/QV2Zm5lCZmZlhvP1Zkz0vT7XjNtDtzHR3lj46kyxlIwd19fuZxyzLYn/KJ5HANbGiBB0jWIyxX16r+cvi348TsfJHKzzRsFi4xPQG8pYSidGJfEbhOV5ZRcXE64nIKkFx75T1fOvq/lFPzuzGpoqK5oSEhJaKCrvD2ZiN9ztvl9veknA4wa6NkOuqba14VHo4vqVC29auZs/VYb97mEiCvbOjFgfEEc7u6rbbEzSvAR3usXf0PgyQpOybjoqWeOJLT8XVPla28Uhjf7dzIJw9AJxw3+CdhIShiqsx+5/bkUYiafjgXhQIN+IXX6acLygsINGJCMHputEnua43Xbcpf0NQjU7tXcMjgBjSE0E/B0CPOrvCVZHCo2NiWgrHoPFQaku4dkLSwCBgXtsnvdQ3DndPASMcLxkcudMV6ycDR7R/jQAwkhYk2Dtv7yAO+yftk9OHQfRW0lIaSZWOiGgNIGEL9Mw8DBan2XhADPKpQdbROdEdONyUFhxOp+jbIvKaICVjpCEvI0P8yJt6gtf7Vm3lAm/ejZOI+bz3e6eYcR3y5CwtAGjMveC97ZBhoPZjJjxZ8NB9frcSBFGTWAbT4ghtCYSd4NqSH0BVR06AmDMyUwz6a9kfoNIWr8mVANJFxkmJNqijdK9EBIVTzAplCa3uyVM0wK+Ueb0WTFOb9EcWXm/iyFtKkSVhyH1uNFh8qsDCWXkTwak9zqN3XSApzutp3kGMuiBmZDBUedDDO7tzEa5Jae2aUiggfcsGTbZTo8gFEIaJSd9IwJt/gRD6vPqLYGLGV6AWBkeIWQVODGyliXbOAV3AXl+LCAKn5W3aDoJz6k6y0YJZT+JpBoL4BHyJuim6YDEastIlnpKTnhv0BqvRF+EMFu7G0ZCFHY/e9x9eicFvfUcW6wd1SMpOVfn4SeHmZ0VgaBEjkFMHSL5FGK6FeQ7dWAFv9sRsncY+3DFtCgr2LAFzm9gXKievAVJRcb6PGBqXxWv0JTZgnAZ08kA6qerYpFxrf+ANYFK8mPS8QX8pqzKdSGXSSwLaRepOXDbHc/TfeFbLdfKZ5eu3dUnt97q6H+3dcXQU1fk38CdN8N4nZFnA14mECljJ3Dubyt7dXY0sSKhZJTZB8fS8zaLatwSj5UT7/sM//M7hD8HznlKoCmCACCBFCYiAmAJKSHwFVICAFdFu5IfGVi2VSCIBTNvmPe9sksTNzt2d2dmNHjz52LaaM7LJ5rvPc+fOnTtvAwTffnPZwDjlNRA0RUN7Y1vdUWMaIr+AEQTlaO0R6HFsDMX4VowGE1k7CSYW6RAvmOcp9zjBuEK1h2CTsYa+YDdOGU2GV9gNcWRkx348nGvBplmqGMjnveGXv/nN/76hIHbYxETkCzzANF+goLSq1M9VlfdFi3u1uQCvLnsVIpTCZbpCPUzLlu0fWIGRogXOehhgE0GDdQAbaPxI5p+AiJOhxEl4txASUV4+SNEcxeX1ZrG8kybMZXbWcGP9rM20EidqjNOfJT9qDsitRsMvZfJt6VorzlQP41z1+Vw8+osuza8JzplHcC6q3jt16tT7pW5vX+QCjI27GcrfXNZTnkb/Qc/RO8siXoUok0IUrTkNAzyDBnRHcP3ChC1zfiRN1CwHKxWIz7GKogWRgxY/D4l8MBkTo8t3oNGz9uI0qhGNXgC5o2hADtuddjLMU4qAcGnuSO0R/TSuelwel1cwD/OL0g9Pfajn6b0KlfFA31zUvHLYv+yJpyK9bf8fesOkWwLfGkbRqlAhRLsejWhOPsH4CJ2owDEL+V0DcWX8BS2jtYcSpclKkSNosNJenI7Jxor5DvnICSVWgC0zFnHJ3b2ax9ddhfr5REVVZVWFmwvOhBY+pfuw+ed6deo9SC0puCvS65568lUof+MdPVYfdQfq1ehWtyaElpEc0ziRHWaNc2pONpo2V+qshjgyR6JlFLEpC+JwZB1EaidN9M6gnTgpZ6SHbQWZ4yjxGtjhuNYlYnDm87EAZ0xExcnf3d7eq3Qzzn0F3X9fWsKYt3fsJHjkZrv973z04ov//fabephefPGpZbo3auBb1bVoHT07cP9jk7dfbvI+QtHcyCBIjd5IMCmvZcTL5VG0aUwQoh2wOBSXxgmPSytwExpRe3GCWX5hwIw3sujtrVuVT3Beof/D+xUB0X1q5+OCce6JrCd444kn9Rh1h+nFJ5+I7XQ12zEZA+O0HO0gSNEcIR+DjOM4weSE4ozDcvehXdm5EO0sWovTLiLLSGOGfJbAiHwCdii3e4U5L39fb27dw6WZfi7875167+clkfqlC7i4h4uCuwGO6GnSdYcpkibdm0eg3wvEfpzkYydMl40gs4om+xIUV4PMC5i2OFFrcTpBUOakrNcR+Vtix9gJehYsKCsrrQxH8lTKhSqaP6zyewO8t8+pHg9/MAj79U7XrydNujeKoceSWkwGOSCP02AguFT+qU1ayJkjqXJLQ/hdV6esWpSZKOl1B1GCDLN7W5SVODHN61JLKpo/PFXFBFMrWjQXE718zK89OgPqP30xyjvL3nzj7bd/X1dXVwM9XqeYDJL53cWJkuFgUN9ECSZvhWJsddfgdx4naJDHfb618zokW+31OqZqFncG83hcntL3SlUWYKqba0IVPfzCfesMCP71xWhP/N/ivkU90O222iTfRsd3GCdsCkKsImIjTYTStRDrGfwe4iR/Ubra2nkd1mbaeqzBIuaRbLmjjXMzlxoIMOFysW8feOAPqGUzvYIzn6r5hY9xxjw+Dxs/NwigZP7tyag4fbSsEAbYSzAeSqlsynsw42QegkN2++Y2JbbvOL+POG0iKNMg6XXpGzrNKuGqLE6ffT6uxOXz8YDbLd+6gGka4wE/97oLJtxcDt2qv+gL1JNPPbHsD/tjBnzE+mwLoaG87zBO1DhUUHbbjRNda7xa/z3EqVjeYWm1pREieQ5sGDuPB5gxTszrf+DvNxYEuMaYUIWMj/sFV72CuR+5Dvot+NtHuife6bnCEp2n3GxpWWo4febk3jMbJr5FSOJLn9enKTYYx3IYaD1Fu84pMcWJYgrG2IwTPEtQ5gULvY6iMwtseFzzBfyxeWFcuIT783/cNM+vah6fX8ioXPh8QrimfDljwLLkujfeXNYvCLolb//hjSWQIfscnzsQhB41eWdGZpP+Xzd5KZhEnChZuJAgpWiOUoIy9Hw5RAueIyhFTadPaSjH9shJ4rGgvTgpmU0oQZuCVnrdaUieMmOexycMcfJoXFXV1q9uGv3lI5pPMCHD9WMKZt7yk7GgxFbZwiNHjrz66qtHjuxXIgHb8OSTHx2RXhy4Jhei1WwaNjFfD8Z/hc4dcMgHjFK1x09+/PHUlU6CiZDFRavPbFlMKEqFlkK0AyGa4E/aeP2+x0j85r0OJGVZFky6EPPza/+LIEl7nHTyJhvTi1+WvjY9BDbc7eYqN+TFwzUh+IW29q8fLr7jwSl+zlSmCcYED3AhhFcTLj1u7lseefCmsQmyqvT9/5Ijvw/CWQtnGQoot9XlLV2SqwBYj9PK+dBt6dFEI5rJu2q6s/5sLcqthWgvxc/SmgU15QA11adDKEcn10QPiePXyTGrjtXV1Iw6snp7/DhR23FScmpRgv4ZohVJj9kGNtw3QcR34WJH+1d3z4CxN8+9cbxWUhLw+Dya8LKSmfePn/KzuV/ePLYYpJRjmWB0lmAs+jTIOcBqnCjFVf1H5x2MmwG6PAt6OD6Wp4CsMbuMpaO0dsNo6PNxdrwBWvRnex3G4dyQ0feZy12Tj0jSXZ1gJErQg5nmP+hhsOF2Pxfx8EsXL7bpBeofMwAKx067Z+6tv4u4de49N08rLOxN0oxpD4PRiXf3g4G8OpkzqU50X1T21sftPw2ZpoXnMkR5nqAMbYq+iUk5kU3lx12BfuWN8UrYNxBlwfY4eaIpxGktRZmppud1TTV2xuEFXhHfpY62trbOzvav774WABTDPYDK2C8fnDduSqFkLuupf+63Up3Q+UqqcaJYbeU68YBZh2qneZyuoJTzUEwdDlF5er+NwAGKUtnVsYvraNqrE+xGmSKzXkcmQfIemufjIi4WiVN7e3tnR8dXX//j2ocgSvFDD3/5968XFfgDgj8uGTS9+uR/l1uoTkjpvkln84rtxEm+Tdb6OEXlJYh2DiXIu9G53C0fOm+AGF/IK0rUafYwlKKGT9LzTkLTXJ3gMEEj0phlsjalsR6SVng7Z4ni5O1o16tTW9vF9rb2i199/vW/Zs26Szdr1t1///rzr9ra2y5oTPWVTBlrzFPh3544AhHm63QoddKmFVd2LS23F6ePIVqN/DJb6ABE2yNtK9PhW/W1FCUWG0pxxhjpC0YNnlagDD1dDrH+RNNenWomowQ5adLrTkPybipwe/wlnJcwpqpCcEOc2qJ1dHS097jY0dHaEYlZq2AlHs5mgZGypA5iZJFEl1gobbxz2KZg0nEK5ZoOPiliPgzwykKU2CFdGRuN7oJYyi75zNQZ6HMQZZwLJPNEtZju6gTPkMRTtscxPbMEwWm3uPweD/eXVpb5hZcZ4qReahugoy1WqxABj6ZJylPfAMvS9Avp/S9SGlq5OTe5OC13wABniNlgQfcBmsVpj7TvNBVKavxRlB36KfSa75R3VkkxVk6nvTpBfaM0zDnf9joqfyeSNGOKx6uqqihraWmZU8BVkzjJdHiFS2ia+x6wZKSlhdK1w7OsxkmyLl/ZKm09fwYLVz12mM06hdaBxOuJ523OoszCV0DiG5L2OEmnMknfGj9lL8G03MNS+G81oPmYn5W26Oa4jdWJXWgzc0lzadzllpQnmb3E4rr7LzItx8kwNs6zFKdas+q0jVLJtyfNwDGKEp9Arz8SlKitB4k66bF3phSnEyGUaEj0scmvS34Y7ipRvX5VZRXNLbpS45UU9UKHaZyEpjJR4r4LrMidjBbVjnBALGUlyjxtGKHRdMTpNVlGnHkgsd8pC3DfoF0ZhtJKWAgSuY+hxF8glTgpE2XvSKg67nkdxVVJp+k/43wuxt2qz1tS1aKrNM5nslbTZndBcJVx4btxNFjxLEGL6J3Gjie9McoQp1Eok2ycCsfI5kMPFlq95RFJfgb0uIwSZAXIKBuRpjtOsJUSyfv2QtyGQUN5SadpTklJoLSlzOfXNHdVd5yM+6l2tJm5ILqpN8wGKxznrC8ZOn/CSpyQrDWNk6Q6ZZjFSXoAvQakfktlBXZUX5woGpF/gtTK1KuTbNmTJE+Ta+KuTdkHSfpPuJS5yz481awFxKVL7rLKygrhYWKgS21W4xTw3A6WZE2maBHNXmIpTodsVCfztIwKocR0kJqOErQvTjtRgvwVpHaS9McJTqIE3QwAmdJetxaS86twuEJztZw6Vamqlzo7L4hIw1K5GOhCm6lW0c0v2bJX7oQTraLns2zFiaDEn6xUp/PQxyGvcU9ZjhNFHJUobGR1nDjhIMSpTjrxfTzeydHRYkiG8uVvwi0i4Aqfer/AF7ikh4K5GFdFIKbXtXZYjZNw8XvAmqXnKbG+ptFOnKjdOJHzZjUuTpwWk0TNThon/GJwmp3cC7KBYFOmrNdRxDWQjOBNN4TDlSUiUNpcURJg3gutlzjX+ICJAjXAuLfDQnXy9q0dnzIarMnaTSlatCttzc5+dTKPU7ARJfL74vQuRSMqHzspK0ziZG/n3izZd0imQkYTNY5aQ/WQhMJ/jSsLv1+q+hlzqV7VpTLORewCOo/fJy50tplob++NE2ds/LVgkeNKCK2h1zi+0ziZVqfpYHmiAMf0n9kRNKIr5Mn87aDECbYQSaYnyq/XrQTrlId/XOCq0odOTGPsUmtH6wUv10T3KksWnaiAT7XQ6/riJDyafxZYlldErd0PTg98b9VJFnlyVL5WECVog5IwTtuLQaLQOShxcmyiskUPOUWpXa5TCu+Z4PO75oS7ZgrNc6l/+MM8nthJ8YCVXtc+rv+ZP75HIQl5Lx1EcwR3fl/VKSMfJWr3W999YBv0uiJtds7fg8TzhKQaJ7mNFI1O52Mql+tG3zXF5RN+d3O4mTPuau2+qtt+gQl/aRnzDBg7idKKzjYzF9t+rvG+RyVMGAvJqFlf1EQomhjjGJQ4OUZRkzgVb0cJcgwkFss++vQ09FqDMuSPYKSMQBycOB2mFjczCp0Bawr/x40FLFCiuvxd4UomNE97TyZa1YA7PMc/YLNnT0nLuM6L5nGq0oTgXDDB1OQf3VpzYM/yySGSqO0tXDI4EwWj0CRO8In0/X9dfsmOoNEZ6LUZpe4sB4PyHYMVp/LJaE1tHVgx9ssbC3jPsHlmOFwqhObtu1CiagVdzR7hj8qTq6Kltd18KN45p8DHuCY8PuEen3ScFAdA8INDe46/VrsQ5ejTg1OdTOOkbEGZ2iUQS1kuTdPCrf2NPSSvC1vB4BUyWHGS1Ei5VWCu/OG752mMix5l4XBZ7yW5ju7LuKKgq8UrAlFxYlWVHRdN43SxvXmmqqpM+Dn33GK12UkWRtU/v6fBKS1Sz6YeJ3vVaQ+itfK0laJE1D20wYMotSMTYtRtp4MWp0wnWkDxBJh56PEfL1KFR3WJbrxUP7HrvbNA/6uVC+Zv6SoR0ficCivFqb3FrXEuPJ4Sj7gVFLDNoUDOFir7sNiIE7VSnYhJnA7JDqA0FPt212ejVIMSPQyW+rQQoim5RYj240QTxwlWoQVkn1ldmnX7BHcgwFU/7x01l5SGu/xCF5nBbL3k5ULlzXrAPAHRz998qd3C0OmzXz8ywa+JAPMVPHIfpGoiGq1LOk5Z8upk5aDz5mvFcXcmRKvbiPLjhkO/SShH/lksf+CMSZwWLLQTpxMUzdFdkNDc8TM540xlPuEW3Zi/Ktwc6N0nXP/LqwkPm6O3P19UnGY262udzE/txv2/8odm/+qnt98+9/FCSNlJNKDJx6maUPtxMt8+hW6sG5gmuYUfR31HVJ4mSlYsiQqJLJjW42Q+/TiSoKnJuYn73ASPn/c+V4z1xon1xSlqJrwqXKUxr+hXMae1zVRne+kx6KOARVl5z5/IyioHo6kkHdXpm4UWqtMH8eJkXlNwef+MUfD57UilIaDRTwwINmEctOmZBcWgK1+wIR8pSSlOv4XEDlE0NQwSKn6Ac8NToSvDc2K/WBGZiRLfKqtsbTPX8cukO9zUBidZuBBrtx9f/UFspIZJwrJqkOJETeOU48Q4nKvyigGU+R+vCCFFueGy1doSxLnj07/+9dPd8hdLa5zKrzHdNpvWQ0LKvSUuEasyXBkbJ3dLuCJ67FRWZWVOvO1/BSEpNSNJ/0OuCN0+fHNGdJc6aPI0gpEo83RycZIfJIkT7MM4KDqvObe8IXth3DDhwBH7JoqJUYpJxGmJPE6LwcRhSjGxFWBi2jghiVNVbJw0/WtiQJzaLRSnz34FyTlNkA7Yt6tp955XflRfB1B3YHUtTRyW8t9aitMuQizEiZjH6TCxn4B9MdvYJTraGEqTON2GMmQMmAjmo4lNYOZRd2xyuqMTw1sWbnGzqH+UxMloQgYk5ZjkbaQLqTP7tdfi7BxBqqPi9JhpnCQpsF+dao6iTYSulZ9kmLMfJ8wGM38yCe41YOq+B5hPiJ795Dnngnl5lTFOmtYcrioJMMa17kfUVVTqJ3adiaacOtva238Cydkn/WSSvv+VCOUOSpwcVuIEz9mO08jYK+9jBj9OhIKZeicmtBrMTZvAuUvz+ESgZ9GAK1AVrhQxuKss3FXmEQEu/JwJMbOyM2F5uqinqe3zJAfimSFM1j5IOk670lWdoKYBbaGYBwMph0la4yTrnQTB1DpMpDYXLJg2xe/yeYQ/4BOREsV4mfHMzqX6q8JdFQGXS0QO0irmRMdJkqbO9va2ZIvTeoLJGgHfyl2cvmb3HLEQJzgkO3MzR1ZBLMduJOmLE0EpMFVNMC6KV8AKx9ifTNE0n4u5OFMFF0yfFSgRA3GN6UOqrlK3V9WEflRVZOzUmeCcrrO98/NrITkjMFkkLzpO+ZbidAZl1kjiZH7f005iJ0+f1IB8i+90xalWnk0wN5FYebPNlxPcOsWvud0aE5yr/q4uw/hceFR3aVe4papMVzWnq6L9YvvFhHFqvweSdByTtc0h3THDRpw2SOJkvnKs5pOk00SlD3dRlHWUpilOSu5iSm3G6RViaWBhrvi62XN/9+iNU6Y88MCE34TLBGdMRPG5Alxzl85p1s2pLK1grYm6Xbs+Fv+6EJJ0PSZp4TeQrjitsRUnyHNisugfQSZDslWhzeqknEMZJ5hzbI8f6r2QFAWgvLi8vLh49C/er+SaqjJhpAnOE2930d4eOeVr//wOGOw40W1KuuJErVQnuhFiKWeTjQC9AnI5tUjTEycokv1JoTFgwWqCcWwPQvIUBwCM/nVLgU8EVJHApfhpuqjPIXw12/6KAYsIXQqQ/FD8jOy9JoY4SQ6i08Hom+Qemke+gHjWEpKmOG2Q/ogbwYLR2fIaSXAE2PaLcKmL+yVxMt+goDNyUtf51b/KIWlrkkxTzNlY8E4iGanE7lHgOGm2qlOnyGcTPgWJs5gE8lIxxKNssBunfUrsM1So3RuaHJMoSjkzwba6X7e4OWcikdaOeOWpre2rfxWDjP1ZNImRDvNmKTkfOURQYrOVM+bLIM0TRcsuQwKOVSQtD9GA25xEPjy0oK6WEvk9Q/bN+J/hKu4SCV1qk+epo6Pzczu1KdlHt16TCQMp0upW65A868v0kYAZTbIW9hxIHcin5qNoipRSk4bh2GBWkKmVOCnKRPlj3S1ZRaXf/VKw777x+lSA0AKcqWr88hTn8oq+KXQ52OKwPHoiuCMLYi0g0uWaMYIr0OgcxCiSb34ql3mOonmcMHszmFAOO5EkStNyYh4n6T6wNGqPUAnTjdc3BlOJU8HMlubum3+Fan30dLF7vunzv98BdinHLQ5s6UqHJI0rDGMGSg+Yb/RPCdkLMaqdhoNwJMShwOoQNakrhB4fBWYUONGQaCPHK4esxcmxLXbVHsGXwZJDRDqO3wUpmHa/q6JrjtvTc4FOLuC9FJunyCi88t4ZkIK1k63kqfY5B0jUj6HRAYizOX/wOBIS/Vsi+BIYrKaGrdurIb6cT01WkzQcA0sKn6EEZSiGhsFeYv48u4iloegqJ/8R5UZKk7w4F1Jwx/1+tayryl3CRRzyuaf28S3hX0BKMkbkE4Ik/ieUIu7MArkT2TGHPwMSwaKBoaPS063VIRyg8SwkdOxcKMEM2VSwypFTJI0kyX8Z4sQpOxdirXcO/BH/D1iz1GxRrw2OO+7XVK20q9TvYlqCOKnR9Un/289+Ge6qmg0pKly/kcb9lFOsvZKgaWQdp32tCZE2HAKp4LPZ3a9AEQmh+c+C1KFttK96IdJ91WBmwfBaRGpcsdV4/BgkQVFyXm9EioRG19DG10dBEnGCTZOR9n78CG7bCxZtsTFLYOphvypEoKyllHt5d3DkcWLMe6FDP5XTo6QvSPmsKhyeU+H+KaTKAfPXjGykPb9JGn27fKjp9N5CUBLNwua98FZjCDHU+NbwTRDX6MNbDjpDoZDzYNH6Goij+NC6t5z6Uc7zd15ZClYUHxi+o+cbjyCRKD02/GxN8ncVZkzdcjAU9ce8tae+J+Lx4yTZ5OGg0+k8/5a+6h6syqMosxNS4bhvXHdcKppLWUAwnYgrcOlCq+6znzeHuyor/Czwb0iL8pyXr2zZcV5/R0I6Z+P5o+9efjknCBZkVp/dXF0IZvZXV1fvBxN1C7oPUhTL1xQyDrwy6XKPSS8fiERJAVtyNo+4/Pr06dM/vfxcHkSYVCeJurpMkH/zcVwhKHMAUnLfLUKnegrmVPmFdLIgmressrmrpbLUXcKZKh6YAWmh9KSqbn69rq7Qxr/+fXFEN66U34RojqfRepxsvL78+ft0eRBSoYydIiJUl7u0cmYkI4kwVuLX3AVu/TiX4N5b6iDNHH3v6pCd8jgFIT0moQzZBSn6MeteM+fyBSqqKrgaJ0bC2x06r+Aun8/nYUJlnI9/CIYMDiVX/kTgxWmKkyPb/NZfW+71M+7xc58o0QtUGfNqqgjo/xGqxgKCB/wB4Rr/s0cjyw48QnDBNMFEIMCF4B7zTTCH2LWLoMxESI81FCXICEjV7AKPX+Wix8yZLMB6Zgb0IDHGPK5x8+ZeC6PvXVTCPZ6obek452xCHQwZHPMnowz9C6RFZr58W8VMSJEy+haXvy8k3khL05iPi3EuVb3//nETHrl7diFE3DxPiIDf1b8ZBtPNK4QhgyJ3IiXmW3jZFiwyf6ytXQ+6+IDdC5nm942767rZX86efd9Y6Df2wUUad3k0EcGZxyV8t8OQFJSPGpUbJ01FSOXd6GlIA2WX/DIErYaUKQ8v4swlvuVzc/et5aAYzkOn/WycWuDvzZyH+W+5DobYVr1lDNLtRYfrFWOnux7jIPMhdcGTIYoyIxVIXfGDmlsIxnnPKRz3FPgeeEh+5M0/G89E93aqXqbecHcQhtiUu7PvEpHz3LP1EC13zUGMd2k4OzflLNWvXYFSFDdDOjw0xcuZR+0ZYAdEILBodtzoTZv7wHiXV/P4Cx6ZPZQm23KjN+zSE7X6gwzoUX+4gWA8tAhSMOrklk8+aaglKEd2lENaTFvkE8KlBrSAUH2ewLh7yiG+GQ89fu9Pf3rTdYUwxLZ1UdcodZTQ7G2v79mzZ8O2WoJx0VRmGXOHOwkmdBjSw3GzfnYnmJsL7ncHCkyamCPVSwpDNiNFI6JDSjC+phr7pWk3EoqJbM+FdJk2r4BxX2RPcN+im4IwZFCV34lyBBOi6+zXpuVmN8WT1ZA+M+5+wOXnbp/rkWkwZJCtJWhLKAfsGoGJUWzKhHQaPe3ef//uwTuKYchgK7KZppVgV0Y2mhkOaeVwADh0MGSwOdEO6rRfnKYSNJFfD1elIfMp2kF2gm1bzKJKtsDVaciPCNrRdBvYpTSYVr5quEoNxYli8ihZC7ZljEETQ8XpBx+n1Fud/ImiEo1ZcHUaovzIRpjo8tzBjNMkuFoNyUREikmhTfVgyv48wbZcuGoNWUySjdPBDyAVwd0JX7AxD65eQ54hSDAJxCRN5lbQBGP80Fa4eg1R6mkS1YkgTTlNsJomyNMwuKoNWWU9TRTxqP00mY7FCeIVuLoNqdmdRG2amAmp20lQjk6Cq9wQZVQDRWtCw3LTEuCjRD4q2wtXvyEZRZRYaXTbNkF6ZE2OWU9FKUW6oh5+CIYoU19DShKvmSNNq8shTRyZI2O6KMF9Q6Xph6Nw0hhK4oUpkrSjZ3LTmd/g1pGNpHcClVB69MpS+EEZsvX4QSrZww51ztObytO+Ij9z7YjLp6dPX3H5yvpqAAf8wAxRDjx7/GhjaOCopnHHlU2FMJgU+MEaUphz9o+Thl3uNmzN2ZxiGJKM/w8DcvpWG+I/igAAAABJRU5ErkJggg=='></a>
	  <button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarColor02' aria-controls='navbarColor02' aria-expanded='false' aria-label='Toggle navigation'>
	  <span class='navbar-toggler-icon'></span>
	  </button>
	  <div class='collapse navbar-collapse' id='navbarColor02' style='padding-left:2rem'>
	    <ul class='navbar-nav mr-auto'>
	      <li class='nav-item'>
	        <a class='nav-link' href='/'>{IndexPageTitle}</a>
	      </li>
	      <li class='nav-item'>
	        <a class='nav-link' href='/config'>{ConfigPageTitle}</a>
	      </li>
		  <li class='nav-item'>
		  	<a class='nav-link' href='/update'>{UpdatePageTitle}</a>
		  </li>
	    </ul>
</div></nav>
		)rawliteral";

static const char SMOGLY_LOGO_URL[] PROGMEM = "<a class='navbar-brand' href='/'><img border='0' width='165' height='40' alt='SmoglyLogo' src='https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/Smogly_Logo/SmoglyLogoTopMenu.png'></a>";
//"
#endif

#ifdef ARDUINO_ARCH_ESP8266
static const char WEB_PAGE_FOOTER[] PROGMEM = R"rawliteral(<br><hr><center><a href='http://smogly.org/' target='_blank'>Smogly</a> &#9830; <a href='https://hs-silesia.pl' target='_blank'>Hackerspace Silesia</a> &#9830;
	<script type='text/javascript'>
  		document.write(new Date().getFullYear());
	</script>
	</center></div></main></form>
			<script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
			<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
			<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
			</body></html>
	)rawliteral";
#elif defined ARDUINO_ARCH_ESP32
static const char WEB_PAGE_FOOTER[] PROGMEM = R"rawliteral(<br><hr><center><a href='http://smogly.org/' target='_blank'>Smogly</a> &#9830; <a href='https://hs-silesia.pl' target='_blank'>Hackerspace Silesia</a> &#9830;
	<script type='text/javascript'>
  		document.write(new Date().getFullYear());
	</script>
	</center></div></main></form>
			<script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
			<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
			<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
			</body></html>
	)rawliteral";
#endif
	
// CONFIG PAGE - Start
	const char WEB_CONFIG_PAGE_TOP[] PROGMEM = R"rawliteral(<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - {TEXT_CONFIG_PAGE}</h1></center><br><br>
	)rawliteral";

	const char WEB_CONFIG_PAGE_SUBMIT_DEVICE_BUTTON[] = "<input type='submit' name='submit1' class='btn btn-outline-danger' value='{TEXT_SAVE}' /></form>";

	const char WEB_CONFIG_PAGE_SUBMIT_SERVICES_BUTTON[] = "<input type='submit' name='submit2' class='btn btn-outline-danger' value='{TEXT_SAVE}' /></form>";

	const char WEB_CONFIG_ADVANCED_MQTT_PAGE_SUBMIT_SERVICES_BUTTON[] = "<input type='submit' name='submit3' class='btn btn-outline-danger' value='{TEXT_SAVE}' /></form>";

	const char WEB_CONFIG_PAGE_SELECT[] PROGMEM = "<select name='{key}'>";

	const char WEB_CONFIG_PAGE_SELECTEND[] PROGMEM = "</select><br />";

	const char WEB_CONFIG_PAGE_ADDOPTION[] PROGMEM = "<option value='{value}' {srslyValue}{label}</option>";

	const char WEB_CONFIG_PAGE_TEXTIMPUT[] PROGMEM = "<input type='text' maxlength='255' size='20' name='{key}' value='{value}'> {postfix}<br />";

	const char WEB_CONFIG_PAGE_MQTT_TEXTIMPUT[] PROGMEM = "<input type='text' maxlength='120' size='32' name='{key}' value='{value}'>{postfix}";

	const char WEB_CONFIG_PAGE_PASSWDINPUT[] = "<input type='password' maxlength='255' size='20' name='{key}' value='{value}'> {postfix}<br />";

	const char WEB_CONFIG_PAGE_INTINPUT[] PROGMEM = "<input type='number' step='1' maxlength='255' name='{key}' value='{value}'> {postfix}<br />";

	const char WEB_CONFIG_PAGE_FLOATINPUT[] = "<input type='number' step='0.000001' maxlength='255' name='{key}' value='{value}'> {postfix}<br />";
	/*
	const char WEB_CONFIG_DEVICE_PAGE_BUTTON[] = "<a href='/config_device' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_CONFIG_DEVICE_PAGE}</a>";

	const char WEB_CONFIG_SERVICES_PAGE_BUTTON[] = "<a href='/config_services' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_CONFIG_SERVICES_PAGE}</a>";
	*/
	const char WEB_CONFIG_PAGE_WIFIERASE[] = "<a href='/erase_wifi' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_ERASEWIFICONFIG}</a>";

	const char WEB_CONFIG_PAGE_HOMEKIT_RESET[] = "<a href='/homekit_reset' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_PAGE_HOMEKIT_RESET}</a>";

	const char WEB_CONFIG_PAGE_RESTORECONFIG[] = "<a href='/restore_config' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_RESTORESETTINGS}</a>";

	const char WEB_CONFIG_TOP_PAGE_INFO[] PROGMEM = R"rawliteral(<center><div class='card text-white bg-success mb-3' style='max-width: 30rem;'>
			  <div class='card-body'>
			    <h4 class='card-title'><strong>{TEXT_SAVED}!</strong></h4>
			    <p class='card-text'>{TEXT_POSTCONFIG_INFO}</p>
			  </div></div></center>)rawliteral";

// CONFIG PAGE - END
// CONFIG DEVICE PAGE - Start	
#ifdef ASYNC_WEBSERVER_ON
const char WEB_CONFIG_DEVICE_PAGE_TOP[] PROGMEM = R"rawliteral(<form method='GET' action='/config_device_save'>
	<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - {TEXT_CONFIG_DEVICE_PAGE}</h1></center><br>
	<div style='color: #2f7a2d'> <strong>{TEXT_SAVED}!</strong> - {TEXT_POSTCONFIG_INFO} </div><br><hr><br>
	{TEXT_INSTRUCIONSLINK}<br><br>
	)rawliteral";
#else
const char WEB_CONFIG_DEVICE_PAGE_TOP[] PROGMEM = R"rawliteral(<form method='POST' action='/config_device'>
	<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - {TEXT_CONFIG_DEVICE_PAGE}</h1></center><br>
	{WEB_CONFIG_TOP_PAGE_INFO}
	<br><hr><br>
	{TEXT_INSTRUCIONSLINK}<br><br>
	)rawliteral";
#endif
const char WEB_CONFIG_DEVICE_PAGE_SECOND_THP[] PROGMEM = R"rawliteral(
	<br>
	{TEXT_SECOND_THP_SDA_SCL} <br>
	<b>{TEXT_SECOND_THP_SDA}: </b>{SECOND_THP_SDA}
	<b>{TEXT_SECOND_THP_SCL}: </b>{SECOND_THP_SCL}
)rawliteral";

const char WEB_CONFIG_DEVICE_PAGE_CONFIG[] PROGMEM = R"rawliteral(<b>{TEXT_DEVICENAME}: </b>{device_name} <br>
	<b>{TEXT_DEVICENAMEAUTO}: </b>{DEVICENAME_AUTO}
	<b>{TEXT_SELECTEDLANGUAGE}: </b>{LanguageSelect}
	<hr>
	<b>{TEXT_TEMPHUMIPRESSSENSOR}: </b>{THP_MODELSelect}
	<b>{TEXT_PMSENSOR}: </b>{DUST_MODELSelect}
	<hr>{TEXT_FIRST_THP_SDA_SCL} - {THP_MODEL}<br>
	<b>{TEXT_FIRST_THP_SDA}: </b>{FIRST_THP_SDA}
	<b>{TEXT_FIRST_THP_SCL}: </b>{FIRST_THP_SCL}
	<br>
	<b>{TEXT_SECOND_THP}: </b>{SECOND_THP}
	{WEB_CONFIG_DEVICE_PAGE_SECOND_THP}
	<br>{TEXT_DUST_TX_RX} - {DUST_MODEL}<br>
	<b>{TEXT_DUST_TX}: </b>{DUST_TX}
	<b>{TEXT_DUST_RX}: </b>{DUST_RX}
	<hr>
	<b>{TEXT_FREQUENTMEASUREMENTONOFF}: </b>{FREQUENTMEASUREMENT_Select} {TEXT_FREQUENTMEASUREMENTINFO}<br>
	<b>{TEXT_MEASUREMENTFREQUENCY}: </b>{FREQUENTMEASUREMENT_time}
	<b>{TEXT_AVERAGELASTRESULT}: </b>{NUMBEROFMEASUREMENTS}
	<hr><b>DeepSleep: </b>{DEEPSLEEP_ON} {TEXT_DEEPSLEEPINFO}
	<hr>
	<b>{TEXT_DISPLAYPM1}: </b> {DISPLAY_PM1}
	<b>{TEXT_ALTITUDEINFO}: </b>{MYALTITUDE}
	<hr>
	<b>{TEXT_SECURECONFIGUPDATEPAGE}: </b>{CONFIG_AUTH}
	<b>{TEXT_SECURELOGIN}: </b>{CONFIG_USERNAME}
	<b>{TEXT_SECUREPASSWD}: </b>{CONFIG_PASSWORD}
	{TEXT_SECURELOGOUTINFO}
	<hr>
	<b>Debug: </b>{DEBUG}
	<b>{TEXT_CALIBMETHOD}: </b>{CalibrationModelSelect}
	<b>{TEXT_CALIB1}: </b>{calib1}
	<br>
	<!-- <b>{TEXT_CALIB2}: </b>{calib2} -->
	<b>{TEXT_SOFTWATEVERSION}: </b>{SOFTWAREVERSION}
	<br><b>{TEXT_AUTOUPDATEON}: </b>{AUTOUPDATEON} {TEXT_UPDATEPAGEAUTOUPDATEWARNING}
	<br><b>{TEXT_HOMEKIT_SUPPORT}: </b>{HOMEKIT_SUPPORT_ON}
	<b>{TEXT_HOMEKIT_IS_PAIRED}: </b>{HOMEKIT_PAIRED_RESET}
	
	<hr><center><br>
	{WiFiEraseButton}  {RestoreConfigButton}
	<br><br></center><hr><br><center>
	{SubmitButton}
	</center>
	)rawliteral";
// CONFIG DEVICE PAGE - END
// CONFIG SERVICES PAGE - Start
#ifndef ASYNC_WEBSERVER_ON
const char WEB_CONFIG_SERVICES_PAGE_TOP[] PROGMEM = R"rawliteral(<form method='POST' action='/config_services'>
	<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - {TEXT_CONFIG_SERVICES_PAGE}</h1></center><br>
	{WEB_CONFIG_TOP_PAGE_INFO}
	<br><hr><br>
	{TEXT_INSTRUCIONSLINK}<br><br>
	)rawliteral";

	const char WEB_CONFIG_SERVICES_PAGE_CONFIG[] PROGMEM = R"rawliteral(<b>{TEXT_SENDINGINTERVAL}: </b>{SENDING_FREQUENCY}
		<b>{TEXT_DBSENDINGINTERVAL}: </b>{SENDING_DB_FREQUENCY}
		<hr>
		<b>{TEXT_SMOGLISTSENDING}: </b>{SMOGLIST_ON} {TEXT_SMOGLISTINFO}
		<hr>
		<b>{TEXT_LUFTDATENSENDING}: </b>{LUFTDATEN_ON}
		<b>ChipID: </b>{LUFTDATEN_ChipID}
		<br><b>{THPSENSOR}</b> Sensor PIN: <b>{THPXPIN}</b>
		<br><b>{DUSTSENSOR}</b> Sensor PIN: <b>{DUSTXPIN}</b>
		<hr>
		<b>{TEXT_AQIECOSENDING}: </b>{AQI_ECO_ON}
		<b>{TEXT_AQIECOSERVER}: </b>{AQI_ECO_HOST}
		<b>{TEXT_AQIECOPATH}: </b>{AQI_ECO_PATH}
		<b>{ESP_MODEL} ID: </b>{AQI_ECO_ChipID}
		<hr>
		<b>{TEXT_AIRMONITORSENDING}: </b>{AIRMONITOR_ON}
		<b>{TEXT_AIRMONITORCHARTS}: </b>{AIRMONITOR_GRAPH_ON}
		<b>{TEXT_AIRMONITORCOORDINATESINFO}:
		<br>{TEXT_AIRMONITORLATITUDE}: </b>{LATITUDE}
		<b>{TEXT_AIRMONITORLONGITUDE}: </b>{LONGITUDE}
		<hr>
		<b>{TEXT_THINGSPEAKSENDING}: </b>{THINGSPEAK_ON}
		<b>{TEXT_THINGSPEAKCHARTS}: </b>{THINGSPEAK_GRAPH_ON}
		<b>{TEXT_THINGSPEAK_WRITE_API_KEY}: </b>{THINGSPEAK_API_KEY}
		<b>{TEXT_THINGSPEAKCHANNELID}: </b>{THINGSPEAK_CHANNEL_ID}
		<b>{TEXT_THINGSPEAK_READ_API_KEY}: </b>{THINGSPEAK_READ_API_KEY}
		<hr>
		<b>{TEXT_INFLUXDBSENDING}: </b>{INFLUXDB_ON}
		<b>{TEXT_INFLUXDBVERSION}: </b>{INFLUXDB_VERSION}
		<b>{TEXT_INFLUXDBSERVER}: </b>{INFLUXDB_HOST}
		<b>{TEXT_INFLUXDBPORT}: </b>{INFLUXDB_PORT}
		<b>{TEXT_INFLUXDBNAME}: </b>{INFLUXDB_DATABASE}
		<b>{TEXT_INFLUXDBUSER}: </b>{INFLUXDB_USER}
		<b>{TEXT_INFLUXDBPASSWD}: </b>{INFLUXDB_PASSWORD}
		<b>{TEXT_INFLUXDBORG}: </b>{INFLUXDB_ORG}
		<b>{TEXT_INFLUXDBBUCKET}: </b>{INFLUXDB_BUCKET}
		<b>{TEXT_INFLUXDBTOKEN}: </b>{INFLUXDB_TOKEN}
		<hr>
		<br><center>{AdvancedMQTTConfigButton}</center><br>
		<hr><center><br>
		{WiFiEraseButton}  {RestoreConfigButton}
		<br><br></center><hr><br><center>
		{SubmitButton}
		</center>
		)rawliteral";
#endif
// CONFIG SERVICES PAGE - END
// CONFIG ADVANCED MQTT PAGE - Start
#ifdef ASYNC_WEBSERVER_ON
	const char WEB_CONFIG_ADVANCED_MQTT_PAGE_TOP[] PROGMEM = R"rawliteral(<form method='GET' action='/config_adv_mqtt_save'>
	<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - {TEXT_ADVANCED_MQTT_PAGE}</h1></center><br>
	{WEB_CONFIG_TOP_PAGE_INFO}
	<br><hr><br>
	{TEXT_INSTRUCIONSLINK}<br><br>
	)rawliteral";
#else
	const char WEB_CONFIG_ADVANCED_MQTT_PAGE_TOP[] PROGMEM = R"rawliteral(<form method='POST' action='/config_adv_mqtt'>
	<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - {TEXT_ADVANCED_MQTT_PAGE}</h1></center><br>
	{WEB_CONFIG_TOP_PAGE_INFO}
	<br><hr><br>
	{TEXT_INSTRUCIONSLINK}<br><br>
	)rawliteral";
#endif

	const char WEB_CONFIG_ADVANCED_MQTT_PAGE_CONFIG[] PROGMEM = R"rawliteral(<br><b>{TEXT_MQTTSENDING}: </b>{MQTT_ON}
		<b>{TEXT_MQTTSERVER}: </b>{MQTT_HOST}
		<b>{TEXT_MQTTPORT}: </b>{MQTT_PORT}
		<b>{TEXT_MQTTUSER}: </b>{MQTT_USER}
		<b>{TEXT_MQTTPASSWD}: </b>{MQTT_PASSWORD}
		<br>{TEXT_MQTT_TOPIC_INFO}
		<br><br>
		<b>{TEXT_MQTT_IP_IN_TOPIC}: </b>{MQTT_IP_IN_TOPIC}
		<b>{TEXT_MQTT_DEVICENAME_IN_TOPIC}: </b>{MQTT_DEVICENAME_IN_TOPIC}
		<b>{TEXT_MQTT_SLASH_AT_THE_BEGINNING}: </b>{MQTT_SLASH_AT_THE_BEGINNING}
		<b>{TEXT_MQTT_SLASH_AT_THE_END}: </b>{MQTT_SLASH_AT_THE_END}
		<br>
		<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />
		<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />
		<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />
		<b>{TEXT_PM1_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />
		<b>{TEXT_PM25_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />
		<b>{TEXT_PM10_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />
		<b>{TEXT_AIRQUALITY_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />
		<br>
		<hr><center><br>
		{RestoreConfigButton}
		<br><br></center><hr><br><center>
		{SubmitButton}
		</center>
		)rawliteral";
		//#endif
// CONFIG ADVANCED MQTT PAGE - END
// UPDATE BUTTONS - START

const char WEB_GOTO_CONFIG_ADVANCED_MQTT_PAGE_BUTTON[] PROGMEM = "<a href='/config_adv_mqtt' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_CONFIG_ADV_MQTT}</a>";

const char WEB_UPDATE_BUTTON_MANUALUPDATE[] PROGMEM = "<a href='/update' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_MANUALUPDATEBUTTON}</a>";

// UPDATE BUTTONS - END
// UPDATE PAGE - START
#ifndef ASYNC_WEBSERVER_ON
const char WEB_UPDATE_PAGE_UPDATE[] PROGMEM = R"rawliteral(<main role='main' class='container'><div class='jumbotron'>
	<form id='data' action='/update' method='POST' enctype='multipart/form-data'>
	<center><h1>Smogly - {TEXT_UPDATE_PAGE}</h1></center><br><br>
	{WEB_UPDATE_INFO_WARNING}
	<br><br><div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>{TEXT_SELECTUPDATEFILE}</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>{TEXT_SUBMITUPDATE}</button></div></div></form>
	<br>
	{TEXT_AUTOUPDATEON}: <b>{AUTOUPDATEONSTATUS}</b>
	<br>{TEXT_CURRENTSOFTVERSION}: <b>{SOFTWAREVERSION}</b>
	<br>{TEXT_SERVERSOFTWAREVERSION}: <b>{SERVERSOFTWAREVERSION}</b>
	<br>{TEXT_LATESTAVAILABLESOFT}
	<br>
	<br>{TEXT_CONNECTEDWIFI}: <b>{WiFiSSID}</b>
	<br>{TEXT_WIFIRSSI}: <b>{WiFiRSSI}</b>
	<br>{TEXT_WIFIQUALITY}: <b>{WiFiQuality}</b>
)rawliteral";
#endif
// UPDATE PAGE - END


#ifdef ARDUINO_ARCH_ESP8266
static char GITHUB_LINK[] = "<a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>{TEXT_HERE}</a>";
static char WSPOLRZEDNE_GPS_LINK[] = "<a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>{TEXT_HERE}</a>";
static char LUFTDATEN_LINK[] = "<a title='Sensor.Community' href='https://maps.sensor.community/#7/52.000/19.000' target='_blank'>Sensor.Community</a>";
static char LUFTDATENFORM_LINK[] = "<a title='devices.sensor.community' href='https://devices.sensor.community' target='_blank'>devices.sensor.community</a>";
static char SMOGLIST_LINK[] = "<a title='smoglist.pl' href='http://smoglist.pl' target='_blank'>Smoglist</a>";
static char AIRMONITOR_LINK[] = "<a title='AirMonitor' href='https://airmonitor.pl/prod/mapa' target='_blank'>AirMonitor</a>";
static char AIRMONITORFORM_LINK[] = "<a title='AirMonitor Form' href='https://airmonitor.pl/prod/station_registration' target='_blank'>{TEXT_THEFORM}</a>";
static char LATLONG_LINK[] = "<a title='latlong.net' href='https://www.latlong.net' target='_blank'>{TEXT_HERE}</a>";
static char THINGSPEAK_LINK[] = "<a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>";
static char SMOGOMIERZRELEASES_LINK[] = "<b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>{TEXT_HERE}</a></b>";
static char MADAVI_LINK[] = "<a title='madavi.de' href='https://www.madavi.de/sensor/graph.php' target='_blank'>{TEXT_HERE}</a>";
#elif defined ARDUINO_ARCH_ESP32
static char GITHUB_LINK[] = "<a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>{TEXT_HERE}</a>";
static char WSPOLRZEDNE_GPS_LINK[] = "<a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>{TEXT_HERE}</a>";
static char LUFTDATEN_LINK[] = "<a title='Sensor.Community' href='https://maps.sensor.community/#7/52.000/19.000' target='_blank'>Sensor.Community</a>";
static char LUFTDATENFORM_LINK[] = "<a title='devices.sensor.community' href='https://devices.sensor.community' target='_blank'>devices.sensor.community</a>";
static char SMOGLIST_LINK[] = "<a title='smoglist.pl' href='http://smoglist.pl' target='_blank'>Smoglist</a>";
static char AIRMONITOR_LINK[] = "<a title='AirMonitor' href='https://airmonitor.pl/prod/mapa' target='_blank'>AirMonitor</a>";
static char AIRMONITORFORM_LINK[] = "<a title='AirMonitor Form' href='https://airmonitor.pl/prod/station_registration' target='_blank'>{TEXT_THEFORM}</a>";
static char LATLONG_LINK[] = "<a title='latlong.net' href='https://www.latlong.net' target='_blank'>{TEXT_HERE}</a>";
static char THINGSPEAK_LINK[] = "<a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>";
static char SMOGOMIERZRELEASES_LINK[] = "<b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>{TEXT_HERE}</a></b>";
static char MADAVI_LINK[] = "<a title='madavi.de' href='https://www.madavi.de/sensor/graph.php' target='_blank'>{TEXT_HERE}</a>";
#endif