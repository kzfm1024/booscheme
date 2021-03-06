;;
;; e-sheep.scm - From Philip K. Dick's "Do Androids Dreams of Electric Sheep?"
;;

(define (e-eval exp env cc)
  (cond ((self-evaluating? exp) (cc exp))
        ((variable? exp) (cc (lookup-variable-value exp env)))
        ((quoted? exp) (cc (text-of-quotation exp)))
        ((assignment? exp) (eval-assignment exp env cc))
        ((definition? exp) (eval-definition exp env cc))
        ((if? exp) (eval-if exp env cc))
        ((lambda? exp) 
         (cc (lambda (cont . args)
               (if (null? args)
                   (eval-sequence (lambda-body exp) env cont) ; FIXME?
                   (eval-sequence (lambda-body exp) 
                                  (extend-environment 
                                   (lambda-parameters exp) args env)
                                  cont)))))
        ((begin? exp) (eval-sequence (begin-actions exp) env cc))
        ((macro? exp env)
         (e-eval (macro-expand exp env) env
                 (lambda (expanded)
                   (e-eval expanded env cc))))
        ((application? exp) (e-apply exp env cc))
        (else
         (error "Unknown expression type -- EVAL" exp))))

(define (eval-assignment exp env cc)
  (e-eval (assignment-value exp) env
          (lambda (val)
            (cc (begin
                  (set-variable-value! (assignment-variable exp) val env)
                  'ok)))))

(define (eval-definition exp env cc)
  (e-eval (definition-value exp) env
          (lambda (val)
            (cc (begin
                  (define-variable! (definition-variable exp) val env)
                  'ok)))))

(define (eval-if exp env cc)
  (e-eval (if-predicate exp) env
          (lambda (pred)
            (e-eval (if pred
                        (if-consequent exp)
                        (if-alternative exp))
                    env cc))))

(define (eval-sequence exps env cc)
  (e-eval (first-exp exps) env
          (lambda (val)
            (if (last-exp? exps)
                (cc val)
                (eval-sequence (rest-exps exps) env cc)))))

(define (map-eval exps env cc)
  (if (last-exp? exps)
      (e-eval (first-exp exps) env
              (lambda (x) (cc (list x))))
      (e-eval (first-exp exps) env
              (lambda (x)
                (map-eval (rest-exps exps) env
                          (lambda (y) (cc (cons x y))))))))

(define (e-apply exps env cc)
  (map-eval exps env
            (lambda (exp)
              (apply (operator exp) cc (operands exp)))))

(define (tagged-list? exp tag)
  (if (pair? exp)
      (eq? (car exp) tag)
      #f))

(define (self-evaluating? exp)
  (cond ((number? exp) #t)
        ((string? exp) #t)
        ((boolean? exp) #t)
        (else #f)))

(define (variable? exp) (symbol? exp))

;; quote

(define (quoted? exp)
  (tagged-list? exp 'quote))

(define (text-of-quotation exp) (cadr exp))

;; set!

(define (assignment? exp)
  (tagged-list? exp 'set!))

(define (assignment-variable exp) (cadr exp))

(define (assignment-value exp) (caddr exp))

;; define

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

;; lambda

(define (lambda? exp) (tagged-list? exp 'lambda))

(define (lambda-parameters exp) (cadr exp))
(define (lambda-body exp) (cddr exp))

(define (make-lambda parameters body)
  (cons 'lambda (cons parameters body)))

;; if

(define (if? exp) (tagged-list? exp 'if))

(define (if-predicate exp) (cadr exp))

(define (if-consequent exp) (caddr exp))

(define (if-alternative exp)
  (if (not (null? (cdddr exp)))
      (cadddr exp)
      #f))

;; begin!

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

;; aplication

(define (application? exp) (pair? exp))
(define (operator exp) (car exp))
(define (operands exp) (cdr exp))

;; macro

(define (macro? exp env) 
  (and (variable? (car exp))
       (tagged-list? (lookup-variable-value (car exp) env) 'macro)))

(define (macro-parameters exp) (cadr exp))
(define (macro-body exp) (cddr exp))

(define (macro-expand exp env)
  (let ((m (lookup-variable-value (car exp) env)))
    (cons (make-lambda (macro-parameters m) (macro-body m))
          (cdr exp))))

;; environment

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
  (cond ((list? vars) ; ex. (x y)
         (if (= (length vars) (length vals))
             (cons (make-frame vars vals) base-env)
             (if (< (length vars) (length vals))
                 (error "Too many arguments supplied" vars vals)
                 (error "Too few arguments supplied" vars vals))))
        ((pair? vars) ; ex. (x . args)
         (cons (make-frame (list (car vars) (cdr vars))
                           (list (car vals) (cdr vals)))
               base-env))
        (else ; ex. args
         (cons (make-frame (list vars) (list vals)) base-env))))

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

(define (e-call/cc cc proc)
  (proc cc (lambda (cont val) (cc val))))

(define (setup-environment)
  (let ((initial-env
         (extend-environment (primitive-procedure-names)
                             (primitive-procedure-objects)
                             the-empty-environment)))
    (define-variable! 'call/cc e-call/cc initial-env)
    (define-variable! 'call-with-current-continuation e-call/cc initial-env)
    (define-variable! 'and
      '(macro args
         (if (null? args)
             #t
             (if (null? (cdr args))
                 (car args)
                 (list 'if (car args) (cons 'and (cdr args)) #f))))
      initial-env)
    initial-env))

(define (primitive-procedure-names)
  (map car
       primitive-procedures))

(define (primitive-procedure-objects)
  (map (lambda (proc)
         (lambda (cont . args) (cont (apply (cadr proc) args))))
       primitive-procedures))

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
        (list 'exit exit) ; for gauche & jscheme
        ))

(define (e-sheep)
  (define (prompt-for-input)
    (newline) (newline) (display ";;; M-Eval input:") (newline))
  (define (announce-output)
    (newline) (display ";;; M-Eval value:") (newline))
  (let loop ((env (setup-environment)))
    (prompt-for-input)
    (let ((input (read)))
      (e-eval input 
              env
              (lambda (cc)
                (announce-output)
                (display cc))))
    (loop env)))

(e-sheep)
