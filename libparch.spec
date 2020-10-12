Name:           libparch
Version:        1.0.0
Release:        1%{?dist}
Summary:        Architecture detection and management library
License:        MIT
URL:            https://github.com/rpm-software-management/libparch
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz


# ========== build options ==========

%bcond_with    html
%bcond_with    man

%bcond_without go
%bcond_without perl5
%bcond_without python3
%bcond_without ruby

%bcond_with    sanitizers
%bcond_without tests


# ========== versions of dependencies ==========

%global swig_version 3.0.12


# ========== build requires ==========

BuildRequires:  cmake
BuildRequires:  doxygen
BuildRequires:  gcc

%if %{with html} || %{with man}
BuildRequires:  python3dist(breathe)
BuildRequires:  python3dist(sphinx)
BuildRequires:  python3dist(sphinx-rtd-theme)
%endif

%if %{with sanitizers}
BuildRequires:  libasan-static
BuildRequires:  liblsan-static
BuildRequires:  libubsan-static
%endif


# ========== libparch ==========

%description
Architecture detection and management library

%files
%{_libdir}/libparch.so.*
%license mit.txt


# ========== libparch-devel ==========

%package devel
Summary:        Development files for libparch
Requires:       libparch%{?_isa} = %{version}-%{release}

%description devel
Development files for libparch.

%files devel
%{_includedir}/libparch/
%{_libdir}/libparch.so
%{_libdir}/pkgconfig/libparch.pc
%license COPYING.md
%license mit.txt


# ========== perl5-libparch ==========

%if %{with perl5}
%package -n perl5-libparch
Summary:        Perl 5 for the libparch library.
Provides:       perl(libparch) = %{version}-%{release}
Requires:       libparch%{?_isa} = %{version}-%{release}
BuildRequires:  perl-devel
BuildRequires:  swig >= %{swig_version}
%if %{with tests}
BuildRequires:  perl(strict)
BuildRequires:  perl(Test::More)
BuildRequires:  perl(warnings)
%endif

%description -n perl5-libparch
Perl 5 bindings for the libparch library.

%files -n perl5-libparch
%{perl_vendorarch}/libparch
%{perl_vendorarch}/auto/libparch
%license COPYING.md
%license mit.txt
%endif


# ========== python3-libparch ==========

%if %{with python3}
%package -n python3-libparch
%{?python_provide:%python_provide python3-libparch}
Summary:        Python 3 bindings for the libparch library.
Requires:       libparch%{?_isa} = %{version}-%{release}
BuildRequires:  python3-devel
BuildRequires:  swig >= %{swig_version}

%description -n python3-libparch
Python 3 bindings for the libparch library.

%files -n python3-libparch
%{python3_sitearch}/libparch/
%license COPYING.md
%license mit.txt
%endif


# ========== ruby-libparch ==========

%if %{with ruby}
%package -n ruby-libparch
Summary:        Ruby bindings for the libparch library.
Provides:       ruby(libparch) = %{version}-%{release}
Requires:       libparch%{?_isa} = %{version}-%{release}
Requires:       ruby(release)
BuildRequires:  pkgconfig(ruby)
BuildRequires:  swig >= %{swig_version}

%description -n ruby-libparch
Ruby bindings for the libparch library.

%files -n ruby-libparch
%{ruby_vendorarchdir}/libparch/
%license COPYING.md
%license mit.txt
%endif


# ========== unpack, build, check & install ==========

%prep
%autosetup -p1


%build
%cmake \
    -DPACKAGE_VERSION=%{version} \
    -DPERL_INSTALLDIRS=vendor \
    \
    -DWITH_HTML=%{?with_html:ON}%{!?with_html:OFF} \
    -DWITH_MAN=%{?with_man:ON}%{!?with_man:OFF} \
    \
    -DWITH_GO=%{?with_go:ON}%{!?with_go:OFF} \
    -DWITH_PERL5=%{?with_perl5:ON}%{!?with_perl5:OFF} \
    -DWITH_PYTHON3=%{?with_python3:ON}%{!?with_python3:OFF} \
    -DWITH_RUBY=%{?with_ruby:ON}%{!?with_ruby:OFF} \
    \
    -DWITH_SANITIZERS=%{?with_sanitizers:ON}%{!?with_sanitizers:OFF} \
    -DWITH_TESTS=%{?with_tests:ON}%{!?with_tests:OFF}
%make_build
%if %{with man}
    make doc-man
%endif


%check
%if %{with tests}
    make ARGS="-V" test
%endif


%install
%make_install


%ldconfig_scriptlets


%changelog
