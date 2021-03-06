;;
;; kipple - From Philip K. Dick's "Do Androids Dreams of Electric Sheep?"
;;

(define (k-eval exp env cc)
  (cond ((self-evaluating? exp) (cc exp))
        ((variable? exp) (cc (lookup-variable-value exp env)))
        ;;
        ;; macro
        ;;
        ((quoted? exp) (cc (text-of-quotation exp)))
        ((assignment? exp) (eval-assignment exp env cc))
        ((definition? exp) (eval-definition exp env cc))
        ((if? exp) (eval-if exp env cc))
        ((lambda? exp) 
         (cc (lambda (cont . args)
               (if (null? args)
                   (eval-sequence (lambda-body exp) env cont)
                   (eval-sequence (lambda-body exp) 
                                  (extend-environment 
                                   (lambda-parameters exp) args env)
                                  cont)))))
        ((begin? exp) 
         (eval-sequence (begin-actions exp) env cc))
        ;((cond? exp) (cc (k-eval (cond->if exp) env))) ; NOT YET
        ((application? exp)
         (k-eval-apply exp env cc))
        (else
         (error "Unknown expression type -- EVAL" exp))))

(define (eval-if exp env cc)
  (k-eval (if-predicate exp) env
          (lambda (pred)
            (k-eval (if pred
                        (if-consequent exp)
                        (if-alternative exp))
                    env cc))))

(define (eval-sequence exps env cc)
  (k-eval (first-exp exps) env
          (lambda (val)
            (if (last-exp? exps)
                (cc val)
                (eval-sequence (rest-exps exps) env cc)))))

(define (k-map-eval exps env cc)
  (if (last-exp? exps)
      (k-eval (first-exp exps) env
              (lambda (x) (cc (list x))))
      (k-eval (first-exp exps) env
              (lambda (x)
                (k-map-eval (rest-exps exps) env
                            (lambda (y) (cc (cons x y))))))))

(define (k-eval-apply exps env cc)
  (k-map-eval exps env
              (lambda (exp)
                (apply (operator exp) cc (operands exp)))))

(define (eval-assignment exp env cc)
  (k-eval (assignment-value exp) env
          (lambda (val)
            (cc (begin
                  (set-variable-value! (assignment-variable exp) val env)
                  'ok)))))

(define (eval-definition exp env cc)
  (k-eval (definition-value exp) env
          (lambda (val)
            (cc (begin
                  (define-variable! (definition-variable exp) val env)
                  'ok)))))

(define (self-evaluating? exp)
  (cond ((number? exp) #t)
        ((string? exp) #t)
        ((boolean? exp) #t)
        (else #f)))

(define (quoted? exp)
  (tagged-list? exp 'quote))

(define (text-of-quotation exp) (cadr exp))

(define (tagged-list? exp tag)
  (if (pair? exp)
      (eq? (car exp) tag)
      #f))

(define (variable? exp) (symbol? exp))

(define (assignment? exp)
  (tagged-list? exp 'set!))

(define (assignment-variable exp) (cadr exp))

(define (assignment-value exp) (caddr exp))

(define (definition? exp)
  (tagged-list? exp 'define))

(define (definition-variable exp)
  (if (symbol? (cadr exp))
      (cadr exp)
      (caadr exp)))

(define (definition-value exp)
  (if (symbol? (cadr exp))
      (caddr exp)
      (make-lambda (cdadr exp)
                   (cddr exp))))

(define (lambda? exp) (tagged-list? exp 'lambda))

(define (lambda-parameters exp) (cadr exp))
(define (lambda-body exp) (cddr exp))

(define (make-lambda parameters body)
  (cons 'lambda (cons parameters body)))


(define (if? exp) (tagged-list? exp 'if))

(define (if-predicate exp) (cadr exp))

(define (if-consequent exp) (caddr exp))

(define (if-alternative exp)
  (if (not (null? (cdddr exp)))
      (cadddr exp)
      #f))

(define (make-if predicate consequent alternative)
  (list 'if predicate consequent alternative))

(define (begin? exp) (tagged-list? exp 'begin))

(define (begin-actions exp) (cdr exp))

(define (last-exp? seq) (null? (cdr seq)))
(define (first-exp seq) (car seq))
(define (rest-exps seq) (cdr seq))

(define (sequence->exp seq)
  (cond ((null? seq) seq)
        ((last-exp? seq) (first-exp seq))
        (else (make-begin seq))))

(define (make-begin seq) (cons 'begin seq))

(define (application? exp) (pair? exp))
(define (operator exp) (car exp))
(define (operands exp) (cdr exp))

;;(define (no-operands? ops) (null? ops))
;;(define (first-operand ops) (car ops))
;;(define (rest-operands ops) (cdr ops))


#|
(define (cond? exp) (tagged-list? exp 'cond))

(define (cond-clauses exp) (cdr exp))

(define (cond-else-clause? clause)
  (eq? (cond-predicate clause) 'else))

(define (cond-predicate clause) (car clause))

(define (cond-actions clause) (cdr clause))

(define (cond->if exp)
  (expand-clauses (cond-clauses exp)))

(define (expand-clauses clauses)
  (if (null? clauses)
      #f                                ; no else clause
      (let ((first (car clauses))
            (rest (cdr clauses)))
        (if (cond-else-clause? first)
            (if (null? rest)
                (sequence->exp (cond-actions first))
                (error "ELSE clause isn't last -- COND->IF"
                       clauses))
            (make-if (cond-predicate first)
                     (sequence->exp (cond-actions first))
                     (expand-clauses rest))))))
|#

(define (enclosing-environment env) (cdr env))

(define (first-frame env) (car env))

(define the-empty-environment '())

(define (make-frame variables values)
  (cons variables values))

(define (frame-variables frame) (car frame))
(define (frame-values frame) (cdr frame))

(define (add-binding-to-frame! var val frame)
  (set-car! frame (cons var (car frame)))
  (set-cdr! frame (cons val (cdr frame))))

(define (extend-environment vars vals base-env)
  (if (= (length vars) (length vals))
      (cons (make-frame vars vals) base-env)
      (if (< (length vars) (length vals))
          (error "Too many arguments supplied" vars vals)
          (error "Too few arguments supplied" vars vals))))

(define (lookup-variable-value var env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars)
             (env-loop (enclosing-environment env)))
            ((eq? var (car vars))
             (car vals))
            (else (scan (cdr vars) (cdr vals)))))
    (if (eq? env the-empty-environment)
        (error "Unbound variable" var)
        (let ((frame (first-frame env)))
          (scan (frame-variables frame)
                (frame-values frame)))))
  (env-loop env))

(define (set-variable-value! var val env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars)
             (env-loop (enclosing-environment env)))
            ((eq? var (car vars))
             (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (if (eq? env the-empty-environment)
        (error "Unbound variable -- SET!" var)
        (let ((frame (first-frame env)))
          (scan (frame-variables frame)
                (frame-values frame)))))
  (env-loop env))

(define (define-variable! var val env)
  (let ((frame (first-frame env)))
    (define (scan vars vals)
      (cond ((null? vars)
             (add-binding-to-frame! var val frame))
            ((eq? var (car vars))
             (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (scan (frame-variables frame)
          (frame-values frame))))

(define (k-call/cc cc proc)
  (proc cc (lambda (cont val) (cc val))))

(define (setup-environment)
  (let ((initial-env
         (extend-environment (primitive-procedure-names)
                             (primitive-procedure-objects)
                             the-empty-environment)))
    (define-variable! 'call/cc k-call/cc initial-env)
    (define-variable! 'call-with-current-continuation k-call/cc initial-env)
    initial-env))

(define (primitive-procedure-names)
  (map car
       primitive-procedures))

(define (primitive-procedure-objects)
  (map (lambda (proc)
         (lambda (cont . args) (cont (apply (cadr proc) args))))
       primitive-procedures))

(define input-prompt ";;; M-Eval input:")
(define output-prompt ";;; M-Eval value:")

(define (driver-loop)
  (prompt-for-input input-prompt)
  (let ((input (read)))
    (k-eval input
            the-global-environment
            (lambda (cc)
              (announce-output output-prompt)
              (user-print cc))))
  (driver-loop))

(define (prompt-for-input string)
  (newline) (newline) (display string) (newline))

(define (announce-output string)
  (newline) (display string) (newline))

(define (user-print object) (display object))

(define primitive-procedures
  (list (list 'car car)
        (list 'cdr cdr)
        (list 'cons cons)
        (list 'null? null?)
        (list 'list list)
        (list '+ +)
        (list '- -)
        (list '* *)
        (list '/ /)
        (list '= =)
        (list 'newline newline)
        (list 'display display)
;;      more primitives
        (list 'exit exit) ; for Gauche
        ))

(define the-global-environment (setup-environment))
(driver-loop)

#;(+ 1 (call/cc (lambda (cc)
                (set! old-cc cc)
                (+ 20 (cc 300)))))
