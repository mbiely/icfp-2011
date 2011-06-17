(ns fikdm.compiler.core
  (:use matchure
	clojure.set
	clojure.contrib.def
	clojure.contrib.str-utils))

(defn compile-a [x expr]
  (cond-match expr

	      [?M ?N]
	      (list (list 'S (compile-a x M)) (compile-a x N))

	      ?y
	      (if (= x y)
		'I
		(list 'K y))))

(defn compile-lambda [expr]
  (cond-match expr

	      [?lambda [?x] ?M]
	      (if (= lambda 'fn)
		(compile-a x (compile-lambda M))
		(throw (Exception. (str "Not a valid expression: " expr))))

	      [?M ?N]
	      (list (compile-lambda M) (compile-lambda N))

	      ?x
	      x))

(defn optimize-ski [ski]
  (or
   (if-match [[[?S [?K ?L]] [?M ?x]] ski]
	     (if (and (= S 'S) (= K 'K) (= L 'K) (= M 'K))
	       `(K (K ~x))))
   (if (seq? ski)
     (map optimize-ski ski)
     ski)))

(defn- expand-if-lets [bindings consequent alternative]
  (if (empty? bindings)
    consequent
    (let [[n v & rest] bindings]
      `(if-let [~n ~v]
	 ~(expand-if-lets rest consequent alternative)
	 ~alternative))))

(defmacro- if-lets [bindings consequent alternative]
  (let [alt (gensym 'alt)]
    `(let [~alt (fn [] ~alternative)]
       ~(expand-if-lets bindings consequent (list alt)))))

(defn- alloc-slot [free]
  (let [slot (first free)]
    (assert (number? slot))
    [slot (difference free #{slot})]))

(defn- primitive-card? [x]
  (cond (symbol? x)
	x
	(= x 0)
	'zero))

(defn- gen-primitive? [x s]
  (if-let [card (primitive-card? x)]
    (if (= card 'I)
      [[:left s 'put]]
      [[:left s 'put]
       [:right s card]])))

(defn- highest-bit [x]
  (loop [bit 1
	 i 0]
    (if (> bit x)
      (dec i)
      (recur (bit-shift-left bit 1) (inc i)))))

(defn- gen-number [x s]
  (assert (and (number? x) (>= x 0)))
  (let [highest (highest-bit x)]
    (loop [code (gen-primitive? 0 s)
	   i highest]
      (if (>= i 0)
	(let [add (if (bit-test x i)
		    [[:left s 'succ]]
		    [])
	      shift (if (zero? i)
		      []
		      [[:left s 'dbl]])]
	  (recur (concat code add shift)
		 (dec i)))
	code))))

(defn- gen-simple? [x s]
  (if-let [primitive (gen-primitive? x s)]
    primitive
    (if (number? x)
      (gen-number x s)
      (if-match [[?l ?r] x]
		(if-lets [l-card (primitive-card? l)
			  r-gen (gen-simple? r s)]
			 (concat r-gen
				 [[:left s l-card]])
			 (if-lets [r-card (primitive-card? r)
				   l-gen (gen-simple? l s)]
				  (concat l-gen
					  [[:right s r-card]])
				  nil))))))

(defn- generate-mn [s x-code y-code m-card n-card]
  (concat
   x-code
   y-code
   [[:left s 'K]
    [:left s 'S]
    [:right s m-card]
    [:right s n-card]]))

(declare generate)

(defn- generate-complex [s free x-code y]
  (assert (not (contains? free s)))
  (let [[os os-free] (alloc-slot free)]
    (generate-mn s x-code
		 (concat (generate y os os-free)
			 (generate os 0 nil)
			 [[:left 0 'get]])
		 'get 'zero)))

(defn generate [ski s free]
  (assert (not (contains? free s)))
  (if-let [simple (gen-simple? ski s)]
    simple
    (cond-match ski

		[?x [?M ?N]]
		(let [x-code (generate x s free)]
		  (if-lets [m-card (primitive-card? M)
			    n-card (primitive-card? N)]
			   (generate-mn s x-code [] m-card n-card)
			   (if-let [y-simple (gen-simple? [M N] 0)]
			     (generate-mn s x-code y-simple 'get 'zero)
			     (generate-complex s free x-code [M N]))))

		[?x ?y]
		(generate-complex s free (generate x s free) y)

		?x
		(throw (Exception. (str "Malformed SKI " x))))))

(defn- command-str [prefix command]
  (let [[side slot card] command]
    (case side
	  :left (str prefix "1\n" prefix (name card) "\n" prefix slot "\n")
	  :right (str prefix "2\n" prefix slot "\n" prefix (name card) "\n")
	  (throw (Exception. (str "Malformed command " command))))))

(defn- shell-script [filename commands]
  (spit filename
	(str "#!/bin/bash\n"
	     (apply str (map (fn [command]
			       (str (command-str "echo " command)
				    "read ; read ; read\n"))
			     commands)))))

(defn make-loop [side-effect]
  (let [fn 'fn]
    `(((S I) I)
      (~fn [f]
	((~fn [y]
	   (((S I) I) f))
	 ~side-effect)))))
