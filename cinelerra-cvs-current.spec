# This is not relocatable
%define prefix /usr

Summary: Cinelerra CVS
Name: cinelerra
Version: 1.1.9
Release: 11
License: GPL
Group: X11
URL: http://cvs.cinelerra.org
Packager: Kevin Brosius <cobra@compuserve.com>
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot

%description
Cinelerra unofficial cvs package

%prep
%setup -q

%build
./configure --prefix=%prefix
#./configure
make

%install
rm -rf $RPM_BUILD_ROOT
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc
#%{prefix}/bin/*
%{prefix}/bin/cinelerra
%{prefix}/bin/mplexhi
%{prefix}/bin/mplexlo
%exclude %{prefix}/bin/mpeg3dump
%exclude %{prefix}/bin/mpeg3toc
%exclude %{prefix}/bin/mpeg3cat
#%{prefix}/lib/*
%{prefix}/share/locale/*
%exclude %{prefix}/include/*

%{prefix}/lib/cinelerra/*.so
# remove below for no static libs
# %exclude %{prefix}/lib/cinelerra/*.a
%exclude %{prefix}/lib/cinelerra/*.la

%{prefix}/lib/cinelerra/fonts/*

%{prefix}/lib/libguicast*
%{prefix}/lib/libmpeg3hv*
%{prefix}/lib/libquicktimehv*
%{prefix}/lib/libsndfilehv*
%{prefix}/lib/pkgconfig*


%changelog
* Fri Apr 30 2004 Kevin Brosius <cobra@compuserve.com> - 1.1.9-11
- change bootstrap technique for PNG files
- gang fading in trackcanvas, snap to value with shift
- plugin drag from window fix

* Wed Apr 28 2004 Kevin Brosius <cobra@compuserve.com> - 1.1.9-10
- Link problem with libXv, new rpm

* Tue Apr 27 2004 Kevin Brosius <cobra@compuserve.com> - 1.1.9-9
- RPM build updates, off by one Xv/zoom bug
- button bug for z change with no x/y
- mp3/ogg vorbis speedups for sample rate conversion
- plugin translation fixes, bug 35-automation with dnd in same track
- input variables crash checks, 
- remove static libs

* Wed Apr 21 2004 Kevin <cobra@compuserve.com> - 1.1.9-8
- Getting ready for cvs commit, no fadeout bug fix

* Sat Apr 17 2004 Kevin <cobra@compuserve.com> - 1.1.9-7
- hscroll plugins fix, fadeengine calculation improvement

* Sun Apr 11 2004 Kevin <cobra@compuserve.com> - 1.1.9-6
- cvs debug support, vscroll plugins fix
- dv1394_init usage crash fix, fix pnp playback issue
- lower bit domain speedup, non-smp overlay plugin fix
- track optimization fix

* Sun Apr  4 2004 Kevin <cobra@compuserve.com> - 1.1.9-5
- hscroll crash fix, cursor change fixes, dnd fixes

* Sat Apr  3 2004 Kevin <cobra@compuserve.com> - 1.1.9-4
- diffkey plugin, cache leak fix, latest cvs

* Sat Mar 27 2004 Kevin <cobra@compuserve.com> - 1.1.9-3
- Remove .la & .a files from plugins in package.  Update from cvs.

* Sun Mar 21 2004 Kevin <cobra@compuserve.com> - 1.1.9-1
- Initial build.


