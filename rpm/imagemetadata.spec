Name:       nemo-qml-plugin-imagemetadata

Summary:    Nemomobile image metadata handling QML extension and lib.
Version:    0.0.1
Release:    1
Group:      System/Library
License:    LGPL v2.1
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  qt5-qttools
BuildRequires:  qt5-qttools-linguist
BuildRequires:  qt5-qmake
BuildRequires:  pkgconfig(exiv2)

%description
Image metadata handling

%package devel
Summary:   Development headers.
License:   LGPL v2.1
Group:     System/Library

%description devel
Development headers

%package tests
Summary:   Unit tests
License:   LGPL v2.1
Group:     Application

%description tests
Unit tests for nemo imagemetadata package


%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install

%files
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/org/nemomobile/imagemetadata/*
%{_libdir}/libimagemetadata.so.*

%files devel
%defattr(-,root,root,-)
/usr/include/imagemetadata/*
%{_libdir}/libimagemetadata.so
%{_libdir}/pkgconfig/imagemetadata.pc

%files tests
%defattr(-,root,root,-)
/opt/tests/nemo-qml-plugin-imagemetadata/*

