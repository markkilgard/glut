
{
  if($0 == "\\verbatimlisting(fglut.h.notab)") {
    print "\\begin{verbatim}"
    print "include(fglut.h.notab)"
    print "\\end{verbatim}"
  } else {
    if($0 == "\\verbatimlisting(glut.h.notab)") {
      print "\\begin{verbatim}"
      print "include(glut.h.notab)"
      print "\\end{verbatim}"
    } else {
      print $0
    }
  }
}
