# Minishell_gr

Cas a gerer : 
  -> infile non creer 
  -> variable environnement : ex PATH -> $PATH et "$PATH"
  -> le dernier file et celui qui contient l'information, les autres sont créés : ex echol "lol" > test > test1 > outfile -> seul oufile a l'information
  ->ctrl + \ : cat | ls puis ctrl + \ bash ecrit ^\ puis sort de l'attente de cat
  ->les signaux dans les heredoc
  -> resultat bash pour un ctrl + \ apres une attente de cat :
  	cat | grep q
		gdbgd
		tfnthf^\Quit (core dumped)
	ou
		qqqqqq
		qqqqqq
		^\Quit (core dumped)
  
  Plan d'action : 
  Parse
  	compter le nombre de quote (verifier si c'st un nombre pair)
  isoler les cmd
  trouver la cmd (son chemin et s'il elle existe)
  voir si on peut l'executer  
  builts in (cd (sans option) , pwd, echo + echo -n, export, unset, env, exit)
  gestion et creation des pipes (fichiers)
  gestion du heredoc
  gestion env
  
  

  