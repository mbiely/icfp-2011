open Cards
open Printf

type msg_type =
  | MsgStartup
  | MsgTurn of int
  | MsgPlayer of int
  | MsgWorld of default_world_type
  | MsgQuestionMove
  | MsgQuestionCard
  | MsgQuestionSlot
  | MsgMove of (int * turn)
  | MsgReset of int

let string_of_card = function
  | I-> "I"
  | Zero -> "zero"
  | Succ -> "succ"
  | Dbl -> "dbl"
  | Get -> "get"
  | Put -> "put"
  | S -> "S"
  | K -> "K"
  | Inc -> "inc"
  | Dec -> "dec"
  | Attack -> "attack"
  | Help -> "help"
  | Copy -> "copy"
  | Revive -> "revive"
  | Zombie -> "zombie"

let msg_of_turn = function
  | Left (card, slot) ->
      sprintf "1\n%s\n%i\n" (string_of_card card) slot
  | Right (slot, card) ->
      sprintf "2\n%i\n%s\n" slot (string_of_card card)

let string_of_turn = function
  | Left (card, slot) ->
      sprintf "applied card %s to slot %i" (string_of_card card) slot
  | Right (slot, card) ->
      sprintf "applied slot %i to card %s" slot (string_of_card card)

let rec string_of_expr = function
  | Card c -> string_of_card c
  | Num i -> string_of_int i
  | Lambda (e1, e2) -> "(" ^ (string_of_expr e1) ^ ")(" ^ (string_of_expr e2) ^ ")"
  | Error -> "Error"
  | Sf e -> "S(" ^ (string_of_expr e) ^ ")"
  | Sfg (e1, e2) -> "S(" ^ (string_of_expr e1) ^ ")(" ^ (string_of_expr e2) ^ ")"
  | Kx e -> "K(" ^ (string_of_expr e) ^ ")"
  | AttackI e -> "attack(" ^ (string_of_expr e) ^ ")"
  | AttackIJ (e1, e2) ->
      "attack(" ^ (string_of_expr e1) ^ ")(" ^ (string_of_expr e2) ^ ")"
  | HelpI e -> "help(" ^ (string_of_expr e) ^ ")"
  | HelpIJ (e1, e2) -> "help(" ^ (string_of_expr e1) ^ ")(" ^ (string_of_expr e2) ^ ")"
  | ZombieI e -> "zombie(" ^ (string_of_expr e) ^ ")"

let print_slot i = function
  | 10000, Card I -> ()
  | vir, expr -> fprintf stderr "%i={%i,%s}\n" i vir (string_of_expr expr)

let std_world_printer msg =
  begin
    match msg with
      | MsgStartup -> output_string stderr "Ocaml: The Gathering\n"
      | MsgTurn i -> fprintf stderr "###### turn %i\n" i
      | MsgPlayer i -> fprintf stderr "*** player %i's turn, with slots:\n" i
      | MsgWorld world ->
	  Array.iteri print_slot (fst world);
	  output_string stderr "(slots {10000,I} are omitted)\n"
      | MsgQuestionMove ->
	  output_string stderr "(1) apply card to slot, or (2) apply slot to card?\n"
      | MsgQuestionCard ->
	  output_string stderr "card name?\n"
      | MsgQuestionSlot ->
	  output_string stderr "card slot?\n"
      | MsgMove (i, turn) ->
	  fprintf stderr "player %i %s\n" i (string_of_turn turn)
      | MsgReset i ->
	  fprintf stderr "slot %i reset to I\n" i
  end;
  flush stderr

let quiet_printer _ =
  ()
