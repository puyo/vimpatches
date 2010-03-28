task 'default' => 'build'

desc 'Configure to build vim with gtk2 gui enabled'
task 'configure-gvim' do
  sh format('cd vim72 && CFLAGS=-g ./configure --prefix=/usr --enable-child --enable-gui=gtk2 --enable-rubyinterp --with-global-runtime=%s', Dir.pwd + '/vim72/runtime')
end

desc 'Build vim'
task :build do
  sh 'cd vim72 && make -j3'
end
