desc 'Configure to build vim with gtk2 gui enabled'
task 'configure-gvim' do
  sh 'cd vim72 && ./configure --prefix=/usr --enable-child --enable-gui=gtk2 --enable-rubyinterp'
end

desc 'Build vim'
task :build do
  sh 'cd vim72 && make -j3'
end
