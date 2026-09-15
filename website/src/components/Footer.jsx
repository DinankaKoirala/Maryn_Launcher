import { Link } from 'react-router-dom'
import { FaGithub } from 'react-icons/fa'

export default function Footer() {
    return (
        <footer className="bg-zinc-900 border-t border-zinc-800 px-8 py-8 mt-auto">
            <div className="max-w-4xl mx-auto flex flex-col sm:flex-row items-center justify-between gap-4">

                <p className="text-zinc-400 text-sm">
                    © 2025 Maryn Launcher. Open source under the Apache-2.0 license.
                </p>

                <div className="flex items-center gap-6 text-sm text-zinc-400">
                    <Link to="/download" className="hover:text-white transition-colors">Download</Link>
                    <Link to="/about" className="hover:text-white transition-colors">About</Link>
                    
                        <a href="https://github.com/DinankaKoirala/Maryn_Launcher"
                        target="_blank"
                        rel="noopener noreferrer"
                        className="hover:text-white transition-colors flex items-center gap-1"
                    >
                        <FaGithub className="w-4 h-4" />
                        GitHub
                    </a>
                </div>

            </div>
        </footer>
    )
}