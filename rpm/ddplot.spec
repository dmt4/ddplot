Name:           ddplot
License:        GPLv2+
Group:          Productivity/Scientific/Other
Summary:        Visualization of dislocations by two-dimensional differential displacement maps
Version:        4.0
Release:        0
Source:         %{name}-%{version}.tar.xz
BuildRequires:  libqt4-devel update-desktop-files
Url:           http://groger.ipm.cz/download/ddplot/ddplot.html
Url:           https://github.com/dmt4/ddplot
BuildRoot:     %{_tmppath}/%{name}-%{version}-build


%description
This simple program visualizes relative displacements in dislocated crystals using differential displacement maps. In this projection, the displacements are depicted as arrows whose lengths correspond to the relative displacements of the neighboring atoms. The output is given as a Postscript figure, animation, .XYZ file for JMol or standard .CFG for AtomEye.

Main features:
 * plotting of atomic structure
 * visualization of screw and edge components around a dislocation line
 * projection of relative displacements into an arbitrary direction
 * scaling of arrows (for the plotting of edge components)
 * automatic finding of the first to fifth nearest neighbors
 * unrelaxed/relaxed configuration
 * coordinate transformations
 * output to Postscript, .XYZ (JMol), standard .CFG (AtomEye)
 * group loading of a set of plot files
 * internal .DD format containing the coordinate system, position of the dislocation, lattice parameter, etc.
 * creating a .GIF animation from the uploaded blocks
 * changing colors, line thickness and radii of atoms and arrows
 * distinguishing atomic layers or atomic types
 * user-defined settings can be stored in ~/.ddplot or loaded at the start

%prep
%setup -q 

%build
qmake 
make %{?_smp_mflags}

%install

install -D -m 755 -s bin/%{name} %{buildroot}%{_bindir}/%{name}
install -D -m 644 ico/%{name}-ico.png %{buildroot}%{_datadir}/pixmaps/%{name}.png
install -D -m 755 ico/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop

%suse_update_desktop_file %{name}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc HISTORY README dot_ddplot
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png


%changelog
